#include "character_gd3d.hpp"

void CharacterGD3D::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("rotate_camera", "rotation_Vector2","mouse_sensitivity","is_inverted"), &CharacterGD3D::rotate_camera);
    ClassDB::bind_method(D_METHOD("rotate_camera_mouse", "event", "mouse_sensitivity", "is_inverted"), &CharacterGD3D::rotate_camera_mouse);
    ClassDB::bind_method(D_METHOD("character_move","delta","input_Vector2","is_aiming","speed",
                    "move_lerp","turn_speed","camera_predict", "camera_predict_speed"), &CharacterGD3D::character_move);

    ClassDB::bind_method(D_METHOD("set_aim_collision_mask","aim_collision_mask"), &CharacterGD3D::set_aim_collision_mask);
    ClassDB::bind_method(D_METHOD("get_aim_collision_mask"), &CharacterGD3D::get_aim_collision_mask);

    ClassDB::bind_method(D_METHOD("on_parented","child"), &CharacterGD3D::on_parented);
    ClassDB::bind_method(D_METHOD("on_unparented","child"), &CharacterGD3D::on_unparented);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "aim_collision_mask", PROPERTY_HINT_LAYERS_3D_PHYSICS), "set_aim_collision_mask", "get_aim_collision_mask");
}


void CharacterGD3D::_notification(int p_what)
{
    switch (p_what) {
        case NOTIFICATION_ENTER_TREE:
        {
            initialized = false;
            p_camera = nullptr;
            gravity = 9.8f;
            if (ProjectSettings::get_singleton()->has_setting("physics/3d/default_gravity"))
            {
                gravity = float(ProjectSettings::get_singleton()->get_setting("physics/3d/default_gravity"));
                WARN_PRINT("Got gravity from user settings physics/3d/default_gravity");
            }
            //This will be removed if add_child_notify is exposed in godot-cpp
            if (!is_connected("child_entered_tree", Callable(this, "on_parented")))
                connect("child_entered_tree", Callable(this, "on_parented"));
            if (!is_connected("child_exiting_tree", Callable(this, "on_unparented")))
                connect("child_exiting_tree", Callable(this, "on_unparented"));
        } break;
        case NOTIFICATION_EXIT_TREE:
        {
            //This will be removed if add_child_notify is exposed in godot-cpp
            if (is_connected("child_entered_tree", Callable(this, "on_parented")))
                disconnect("child_entered_tree", Callable(this, "on_parented"));
            if (is_connected("child_exiting_tree", Callable(this, "on_unparented")))
                disconnect("child_exiting_tree", Callable(this, "on_unparented"));
            p_camera = nullptr;
        } break;

    }
}

void CharacterGD3D::rotate_camera_mouse(const Ref<InputEvent>& p_event, const float mouse_sensitivity, const bool is_inverted)
{
    Ref<InputEventMouseMotion> m = p_event;
    
    if (!initialized || !m.is_valid()) return;

    Vector2 motion = m->get_relative();
    float xmov = Math::deg_to_rad(motion.x);
    if (is_inverted) xmov *= -1;
    camera_boon.rotate(Vector3(0, 1, 0), xmov * mouse_sensitivity);
    p_camera->set_position(camera_follow_position + camera_boon);
    p_camera->look_at(camera_follow_position);
}

void CharacterGD3D::rotate_camera(const Vector2& motion,const float mouse_sensitivity,const bool is_inverted)
{
    if (!initialized) return;

    float xmov = Math::deg_to_rad(motion.x);
    if (is_inverted) xmov *= -1;
    camera_boon.rotate(Vector3(0, 1, 0), xmov * mouse_sensitivity);
    p_camera->set_position(camera_follow_position + camera_boon);
    p_camera->look_at(camera_follow_position);
}

void CharacterGD3D::character_move(const double delta, const Vector2& input_dir, const bool is_aiming,
        const float speed, const float move_lerp, const float turn_speed,
        const float camera_predict, const float camera_predict_speed)
{
    if (!initialized) return;


    Vector3 vel = get_velocity();
    bool floored = is_on_floor();

    if (!floored)  vel.y -= gravity * delta;

    Vector3 direction = (p_camera->get_transform().basis.xform(Vector3(input_dir.x, 0, input_dir.y))).normalized();
    Vector3 lookat_pos = get_position();
    Vector3 front_pos = -get_global_transform().get_basis().get_column(2);
    if (direction.is_zero_approx())
    {
        vel.x = Math::move_toward(vel.x, 0, speed) ;
        vel.z = Math::move_toward(vel.z, 0, speed) ;
        lookat_pos.x += front_pos.x;
        lookat_pos.z += front_pos.z;
    }
    else
    {
        vel.z = Math::lerp(vel.z, direction.z * speed, move_lerp);
        vel.x = Math::lerp(vel.x, direction.x * speed, move_lerp);
        lookat_pos.x += direction.x;
        lookat_pos.z += direction.z;
    }

    handle_aim(is_aiming, lookat_pos);

    camera_follow_position = camera_follow_position.lerp(camera_predict * direction.length_squared()
        * front_pos + get_position(), camera_predict_speed * delta);

    //Setting the resulting logic to the character and camera nodes
    set_velocity(vel);
    move_and_slide();
    //Aiming
    if(!is_aiming)
    {
        lookat_position = lookat_position.lerp(lookat_pos, turn_speed);
    }
    else
    {
        lookat_position = lookat_pos;
    }

    look_at(Vector3(lookat_position.x, get_position().y,lookat_position.z));
    Vector3 cam_pos =  camera_follow_position + cam_height + camera_boon;
    p_camera->set_position(cam_pos);
    p_camera->look_at(camera_follow_position + cam_height);
}

void CharacterGD3D::handle_aim(const bool aiming,Vector3& lookat_pos)
{
    if (!aiming)
    {
        handle_aim_node(nullptr);
        return;
    }

    Vector2 mouse_pos = get_viewport()->get_mouse_position();
    Ref<PhysicsRayQueryParameters3D> ray = PhysicsRayQueryParameters3D::create(
        p_camera->project_ray_origin(mouse_pos),
        p_camera->project_ray_normal(mouse_pos) * 1000, aim_collision_mask);
    Dictionary ray_dict = PhysicsServer3D::get_singleton()->space_get_direct_state(
        get_world_3d()->get_space())->intersect_ray(ray);

    if (ray_dict == Dictionary())
    {
        handle_aim_node(nullptr);
        return;
    }
    Vector3 pos = ray_dict["position"];
    lookat_pos.x = pos.x;
    lookat_pos.z = pos.z;

    handle_aim_node(ray_dict["collider"]);
}
void CharacterGD3D::handle_aim_node(Object* nd)
{
    if (old_aim_node == nd) return;

    if(nd)
        nd->notification(NOTIFICATION_GD3D_AIMED);

    if (old_aim_node)
        old_aim_node->notification(NOTIFICATION_GD3D_UNAIMED);

    old_aim_node = nd;
}

void CharacterGD3D::set_aim_collision_mask(const uint32_t set)
{
    aim_collision_mask = set;
    ProjectSettings::get_singleton()->set_setting("gd3d/aim_ignore_mask", aim_collision_mask);
}
uint32_t CharacterGD3D::get_aim_collision_mask() const { return aim_collision_mask; }

void CharacterGD3D::on_parented(Variant v_child)
{
    Node* p_child = cast_to<Node>(v_child);
    {
        Camera3D* c = cast_to<Camera3D>(v_child);
        if (c && !p_camera)
        {
            p_camera = c;
            p_camera->set_as_top_level(true);
            //Find intersect between character plane and camera to set the boon and the camera height so
            // it respects the positions of the camera in the editor
            Vector3 cam_forward = -p_camera->get_global_transform().get_basis().get_column(2);
            Vector3 char_plane_normal = -get_global_transform().get_basis().get_column(2);
            float determ = cam_forward.dot(char_plane_normal);
            if(Math::abs(determ) < 0.01f)
            {
                WARN_PRINT("Camera is too perpedicular to the character");
                camera_boon = p_camera->get_position() - get_position();
                cam_height.zero();
            }
            else
            {
                float dist = (get_position() - p_camera->get_position()).dot(char_plane_normal) / determ;
                Vector3 intersect = p_camera->get_position() + cam_forward * dist;
                cam_height.zero();
                cam_height.y = intersect.y;
                camera_boon = p_camera->get_position() - intersect;
            }
        }
    }
    if (p_camera) initialized = true;
    else initialized = false;
    update_configuration_warnings();
}
void CharacterGD3D::on_unparented(Variant v_child)
{
    
    Node* p_child = cast_to<Node>(v_child);
    {
        Camera3D* c = cast_to<Camera3D>(v_child);
        if (c) p_camera = nullptr;
    }
   
    if (p_camera) initialized = true;
    else initialized = false;
    update_configuration_warnings();
}
PackedStringArray CharacterGD3D::_get_configuration_warnings() const {
    PackedStringArray warnings = Node::_get_configuration_warnings();
    
    if(!p_camera) {
        warnings.push_back("No Camera3D node as child.");
    }
  
    return warnings;

}

//Not exposed in godot cpp
void CharacterGD3D::add_child_notify(Node* p_child)
{
   // on_parented(p_child);
}
void CharacterGD3D::remove_child_notify(Node* p_child)
{
  //  on_unparented(p_child);
}