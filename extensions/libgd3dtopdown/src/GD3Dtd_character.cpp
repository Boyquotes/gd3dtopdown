#include "GD3Dtd_character.hpp"

GD3Dtd_character::GD3Dtd_character()
{
    initialized = false;
}

GD3Dtd_character::~GD3Dtd_character(){}

#define GETSET_GD3D(FUNC) ClassDB::bind_method(D_METHOD("get_"#FUNC), &GD3Dtd_character::get_##FUNC);\
                        ClassDB::bind_method(D_METHOD("set_"#FUNC, #FUNC), &GD3Dtd_character::set_##FUNC)
#define ADDPROP_GD3D(PROP,TYPE) ADD_PROPERTY(PropertyInfo(Variant::TYPE, #PROP), "set_"#PROP, "get_"#PROP)
void GD3Dtd_character::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("character_init"), &GD3Dtd_character::character_init);
    ClassDB::bind_method(D_METHOD("rotate_camera", "rotation_Vector2"), &GD3Dtd_character::rotate_camera);
    ClassDB::bind_method(D_METHOD("rotate_camera_aimlock","rotation_Vector2"), &GD3Dtd_character::rotate_camera_aimlock);
    ClassDB::bind_method(D_METHOD("rotate_camera_mouse", "event"), &GD3Dtd_character::rotate_camera_mouse);
    ClassDB::bind_method(D_METHOD("rotate_camera_mouse_aimlock","event"), &GD3Dtd_character::rotate_camera_mouse_aimlock);
    ClassDB::bind_method(D_METHOD("character_move","delta","input_Vector2","is_aiming","speed","move_lerp"), &GD3Dtd_character::character_move);

    ClassDB::bind_method(D_METHOD("get_character_forward"), &GD3Dtd_character::get_character_forward);
    ClassDB::bind_method(D_METHOD("get_character_direction"), &GD3Dtd_character::get_character_direction);
    ClassDB::bind_method(D_METHOD("get_character_direction_plane"), &GD3Dtd_character::get_character_direction_plane);
    ClassDB::bind_method(D_METHOD("get_character_movement_dot"), &GD3Dtd_character::get_character_movement_dot);
    ClassDB::bind_method(D_METHOD("get_lookat_position"), &GD3Dtd_character::get_lookat_position);
    ClassDB::bind_method(D_METHOD("get_aim_node"), &GD3Dtd_character::get_aim_node);

    ClassDB::bind_method(D_METHOD("enter_visual_event"), &GD3Dtd_character::enter_visual_event);
    ClassDB::bind_method(D_METHOD("exit_visual_event"), &GD3Dtd_character::exit_visual_event);
    ClassDB::bind_method(D_METHOD("enter_interior_event"), &GD3Dtd_character::enter_interior_event);
    ClassDB::bind_method(D_METHOD("exit_interior_event"), &GD3Dtd_character::exit_interior_event);
    
    GETSET_GD3D(mouse_sensitivity);
    GETSET_GD3D(turn_speed);
    GETSET_GD3D(instant_turn_aiming);
    GETSET_GD3D(invert_camera_movement);
    GETSET_GD3D(camera_boon);
    GETSET_GD3D(camera_predict);
    GETSET_GD3D(camera_predict_speed);
    GETSET_GD3D(interiors_collision_mask);
    GETSET_GD3D(visual_collision_mask);
    GETSET_GD3D(aim_collision_mask);
    GETSET_GD3D(camera_node);
    GETSET_GD3D(interiors_collision_area_node);
    GETSET_GD3D(visual_collision_area_node);

    ADDPROP_GD3D(mouse_sensitivity, FLOAT);
    ADDPROP_GD3D(turn_speed, FLOAT);
    ADDPROP_GD3D(instant_turn_aiming, BOOL);
    ADDPROP_GD3D(invert_camera_movement, BOOL);
    ADDPROP_GD3D(camera_boon, VECTOR3);
    ADDPROP_GD3D(camera_predict, FLOAT);
    ADDPROP_GD3D(camera_predict_speed, FLOAT);
    ADDPROP_GD3D(camera_node, NODE_PATH);
    ADDPROP_GD3D(interiors_collision_area_node, NODE_PATH);
    ADDPROP_GD3D(visual_collision_area_node, NODE_PATH);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "interiors_collision_mask", PROPERTY_HINT_LAYERS_3D_PHYSICS), "set_interiors_collision_mask", "get_interiors_collision_mask");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "visual_collision_mask", PROPERTY_HINT_LAYERS_3D_PHYSICS), "set_visual_collision_mask", "get_visual_collision_mask");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "aim_collision_mask", PROPERTY_HINT_LAYERS_3D_PHYSICS), "set_aim_collision_mask", "get_aim_collision_mask");
}
#undef ADDPROP_GD3D
#undef GETSET_GD3D

#ifdef DEBUG_ENABLED
    #define DEBUG_WARN_GD3D(msg) WARN_PRINT(msg)
#else
    #define DEBUG_WARN_GD3D(msg) 
#endif
//Initializer functions, get pointers to Input and project settings. Set gravity value
void GD3Dtd_character::character_init()
{
    if (initialized) return;
    initialized = true;
    gravity = 9.8f;
  
    ProjectSettings* p_settings = ProjectSettings::get_singleton();
    if (p_settings == nullptr) { character_uninit(); ERR_FAIL_COND_MSG(false, "Could not obtain project settings pointer, didnt initialize"); }
    
    Variant grav = p_settings->get_setting("physics/3d/default_gravity");
    if (grav.get_type() == Variant::INT || grav.get_type() == Variant::FLOAT)
    {
        gravity = float(grav);
        DEBUG_WARN_GD3D("Got gravity from user settings physics/3d/default_gravity");
    }
    
    set_camera_node(camera_node);
    
    if (camera == nullptr) { character_uninit(); ERR_FAIL_COND_MSG(false, "Could not obtain camera pointer, wont initialize"); }

    set_visual_collision_area_node(visual_collision_area_node);
    
    if (visual_collision_area == nullptr) { character_uninit(); ERR_FAIL_COND_MSG(false, "Could not obtain visual_collision_area pointer, didnt initialize"); }

    set_interiors_collision_area_node(interiors_collision_area_node);
    
    if (interiors_collision_area == nullptr) { character_uninit(); ERR_FAIL_COND_MSG(false, "Could not obtain interiors_collision_area pointer, didnt initialize"); }
    
    if ((visual_collision_mask & aim_collision_mask) != 0)
    {
        DEBUG_WARN_GD3D("Visual_collision_mask and aim_collision_mask share mask(s), this might lead to unexpected behavior");
    }
    if ((interiors_collision_mask & aim_collision_mask) != 0)
    {
        DEBUG_WARN_GD3D("interiors_collision_mask and aim_collision_mask share mask(s), this might lead to unexpected behavior");
    }
    DEBUG_WARN_GD3D("Initialized GD3D with gravity: " + String::num_real(gravity));
}
#undef DEBUG_WARN_GD3D

void GD3Dtd_character::character_uninit()
{
    if (!initialized) return;
    interiors_collision_area.reset();
    visual_collision_area.reset();
    camera.reset();
    initialized = false;
}


void GD3Dtd_character::_ready()
{
#ifdef DEBUG_ENABLED
    //if (Engine::get_singleton()->is_editor_hint()) return;
#endif 
    character_init();
}
void GD3Dtd_character::_exit_tree()
{
    character_uninit();
}

#pragma region Camera_input_functions
void GD3Dtd_character::rotate_camera_mouse_aimlock(const Ref<InputEvent>& p_event)
{
    Ref<InputEventMouseMotion> m = p_event;
    
    if (!initialized || !m.is_valid() || is_aiming) return;

    Vector2 motion = m->get_relative();
    float xmov = Math::deg_to_rad(motion.x);
    if (invert_camera_movement) xmov *= -1;
    camera_boon.rotate(Vector3(0, 1, 0), xmov * mouse_sensitivity);
    camera->set_position(camera_follow_position + camera_boon);
    camera->look_at(camera_follow_position);
}
void GD3Dtd_character::rotate_camera_mouse(const Ref<InputEvent>& p_event)
{
    Ref<InputEventMouseMotion> m = p_event;

    if (!initialized || !m.is_valid()) return;
    
    Vector2 motion = m->get_relative();
    float xmov = Math::deg_to_rad(motion.x);
    if (invert_camera_movement) xmov *= -1;
    camera_boon.rotate(Vector3(0, 1, 0), xmov * mouse_sensitivity);
    camera->set_position(camera_follow_position + camera_boon);
    camera->look_at(camera_follow_position);
}
void GD3Dtd_character::rotate_camera(const Vector2& motion)
{
    if (!initialized) return;
   
    float xmov = Math::deg_to_rad(motion.x);
    if (invert_camera_movement) xmov *= -1;
    camera_boon.rotate(Vector3(0, 1, 0), xmov * mouse_sensitivity);
    camera->set_position(camera_follow_position + camera_boon);
    camera->look_at(camera_follow_position);
}
void GD3Dtd_character::rotate_camera_aimlock(const Vector2& motion)
{
    if (!initialized || is_aiming) return;

    float xmov = Math::deg_to_rad(motion.x);
    if (invert_camera_movement) xmov *= -1;
    camera_boon.rotate(Vector3(0, 1, 0), xmov * mouse_sensitivity);
    camera->set_position(camera_follow_position + camera_boon);
    camera->look_at(camera_follow_position);
}
#pragma endregion Camera_input_functions
#pragma region Character_movement_function
void GD3Dtd_character::character_move(const double delta, const Vector2& input_dir, const bool aiming, const float speed, const float move_lerp)
{
    if (!initialized) return;
    //Inputs and environment
    is_aiming = aiming;

    Vector3 vel = get_velocity();
    bool floored = is_on_floor();

    //Logic with inputs
    if (!floored)  vel.y -= gravity * delta;

    Vector3 direction = (camera->get_transform().basis.xform(Vector3(input_dir.x, 0, input_dir.y))).normalized();
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
    if(!is_aiming || !instant_turn_aiming)
    {
        lookat_position = lookat_position.lerp(lookat_pos, turn_speed);
    }
    else
    {
        lookat_position = lookat_pos;
    }
    Vector3 char_pos = get_position();

    look_at(Vector3(lookat_position.x,char_pos.y,lookat_position.z));
    Vector3 cam_pos = camera_follow_position + camera_boon;
    camera->set_position(cam_pos);
    camera->look_at(camera_follow_position);
    visual_collision_area->look_at(cam_pos);

    //Character info
    character_forward = -get_global_transform().get_basis().get_column(2);
    character_direction = vel.normalized();
    character_direction_plane =Vector3(character_direction.x,char_pos.y,character_direction.z);
    character_movement_dot = character_forward.dot(character_direction_plane);
}
#pragma endregion Character_movement_function

#pragma region Character_interaction_functions
void GD3Dtd_character::handle_aim(const bool aiming,Vector3& lookat_pos)
{
    if (!aiming)
    {
        handle_aim_node(nullptr);
        return;
    }

    PhysicsServer3D* ph_server = PhysicsServer3D::get_singleton();
    World3D* w3d = w3d = get_world_3d().ptr();

    if(ph_server == nullptr || w3d == nullptr) return; 

    Vector2 mouse_pos = get_viewport()->get_mouse_position();
    Ref<PhysicsRayQueryParameters3D> ray = PhysicsRayQueryParameters3D::create(
        camera->project_ray_origin(mouse_pos),
        camera->project_ray_normal(mouse_pos) * 1000, aim_collision_mask);
    Dictionary ray_dict = ph_server->space_get_direct_state(
        w3d->get_space())->intersect_ray(ray);

    if (ray_dict == Dictionary())
    {
        handle_aim_node(nullptr);
        return;
    }
    Vector3 pos = ray_dict["position"];
    lookat_pos.x = pos.x;
    lookat_pos.z = pos.z;

    Node3D* nd = cast_to<Node3D>(ray_dict["collider"]); //<- must use cast_to from godot::variant to other types instead of dynamic_cast. Might return a pointer to a parent class instead of nullptr
    handle_aim_node(nd);
}
void GD3Dtd_character::handle_aim_node(Node3D* nd)
{
    if (old_aim_node == nd) return;
    if (old_aim_node != nullptr)
    {
        GD3Dselectable_node* selectable = dynamic_cast<GD3Dselectable_node*>(old_aim_node);
        
        if (selectable != nullptr) selectable->on_unselected();
    }

    old_aim_node = nd;

    if (old_aim_node != nullptr)
    {
        GD3Dselectable_node* selectable = dynamic_cast<GD3Dselectable_node*>(old_aim_node);
        
        if (selectable != nullptr) selectable->on_selected();
    }
}
void GD3Dtd_character::enter_visual_event(Object* body)
{
    GD3Dvisual_obstacle* vis = dynamic_cast<GD3Dvisual_obstacle*>(body);
    if (vis != nullptr) vis->obstacle_entered_char(aim_collision_mask);
}
void GD3Dtd_character::exit_visual_event(Object* body)
{
    GD3Dvisual_obstacle* vis = dynamic_cast<GD3Dvisual_obstacle*>(body);
    if (vis != nullptr ) vis->obstacle_exited_char(aim_collision_mask);
}
void GD3Dtd_character::enter_interior_event(Object* area)
{
    GD3Dinterior_area* in_area = dynamic_cast<GD3Dinterior_area*>(area);
    if (in_area != nullptr )
        in_area->on_enter_area(aim_collision_mask);
}
void GD3Dtd_character::exit_interior_event(Object* area)
{
    GD3Dinterior_area* in_area = dynamic_cast<GD3Dinterior_area*>(area);
    if (in_area != nullptr )
        in_area->on_exit_area(aim_collision_mask);
}
#pragma endregion Character_interaction_functions
#pragma region Getters_and_setters
//Getters and setters
void GD3Dtd_character::set_visual_collision_mask(uint32_t p_mask)
{
    visual_collision_mask = p_mask;
    if (visual_collision_area == nullptr) return;

    visual_collision_area->set_collision_mask(p_mask);
}
uint32_t GD3Dtd_character::get_visual_collision_mask() const
{
    if (visual_collision_area == nullptr) return visual_collision_mask;
    
    return  visual_collision_area->get_collision_mask();
}
void GD3Dtd_character::set_interiors_collision_mask(uint32_t p_mask)
{
    interiors_collision_mask = p_mask;
    if (interiors_collision_area == nullptr) return;

    interiors_collision_area->set_collision_mask(p_mask);
}
uint32_t GD3Dtd_character::get_interiors_collision_mask() const
{
    if (interiors_collision_area == nullptr) return interiors_collision_mask;
    return  interiors_collision_area->get_collision_mask();
}
void GD3Dtd_character::set_camera_node(const NodePath& set)
{ 
    camera_node = set;
    
    if (!camera_node.is_empty())
    {
        camera = get_node<Camera3D>(camera_node);
        camera.set_non_permanency(false);
     
    }
    if (camera == nullptr) 
    {
        WARN_PRINT("default camera");
        camera = memnew(Camera3D);
        camera.set_non_permanency(true);
        add_child(camera.get());
        camera->set_name("Default_camera");
    } 
    
    camera->set_as_top_level(true);
}
NodePath GD3Dtd_character::get_camera_node() const { return camera_node; }

void  GD3Dtd_character::set_interiors_collision_area_node(const NodePath& set)
{
    interiors_collision_area_node = set;
    
    if (!interiors_collision_area_node.is_empty())
    {
        interiors_collision_area = dynamic_cast<Area3D*>(get_node<Area3D>(interiors_collision_area_node));
        interiors_collision_area.set_non_permanency(false);
    }
    if (interiors_collision_area == nullptr)
    {
        interiors_collision_area_node = NodePath();

        interiors_collision_area = memnew(Area3D);
        interiors_collision_area.set_non_permanency(true);
        CollisionShape3D* interiors_collision_col_shp = memnew(CollisionShape3D);
        BoxShape3D* interiors_collision_box = memnew(BoxShape3D);
        add_child(interiors_collision_area.get());
        interiors_collision_area->set_owner(this);
        interiors_collision_area->add_child(interiors_collision_col_shp);
        interiors_collision_col_shp->set_owner(interiors_collision_area.get());
        interiors_collision_col_shp->set_shape(interiors_collision_box);
        interiors_collision_box->set_size(Vector3(0.1f, 0.1f, 0.1f));
        interiors_collision_area->set_position(Vector3(0, 1, 0));

        interiors_collision_area->set_name("Default_interiors_detector");
        
    }
    interiors_collision_area->set_monitoring(true);
    interiors_collision_area->set_monitorable(true);
    interiors_collision_area->set_collision_layer(0);
    interiors_collision_area->set_collision_mask(interiors_collision_mask);

    interiors_collision_area->connect("area_entered", Callable(this, "enter_interior_event"));
    interiors_collision_area->connect("area_exited", Callable(this, "exit_interior_event"));
}
NodePath  GD3Dtd_character::get_interiors_collision_area_node() const{ return interiors_collision_area_node; }
void  GD3Dtd_character::set_visual_collision_area_node(const NodePath& set)
{
    visual_collision_area_node = set;

    if (!visual_collision_area_node.is_empty())
    {
        visual_collision_area = dynamic_cast<Area3D*>(get_node<Area3D>(visual_collision_area_node));
        visual_collision_area.set_non_permanency(false);
    }
    if(visual_collision_area == nullptr)
    {
        visual_collision_area = memnew(Area3D);
        visual_collision_area.set_non_permanency(true);
        CollisionShape3D* visual_col_shp = memnew(CollisionShape3D);
        BoxShape3D* visual_box = memnew(BoxShape3D);
        add_child(visual_collision_area.get());
        visual_collision_area->set_owner(this);
        visual_collision_area->add_child(visual_col_shp);
        visual_col_shp->set_owner(visual_collision_area.get());
        visual_col_shp->set_shape(visual_box);
        visual_collision_area->set_position(Vector3(0, 1, 0));
        visual_box->set_size(Vector3(0.1f, 0.1f, camera_boon.length()));
        visual_col_shp->set_position(Vector3(0, 0, -camera_boon.length() / 2.0));

        visual_collision_area->set_name("Default_visual_detector");
    }

    visual_collision_area->set_monitoring(true);
    visual_collision_area->set_monitorable(true);
    visual_collision_area->set_collision_layer(0);
    visual_collision_area->set_collision_mask(visual_collision_mask);
    visual_collision_area->connect("body_entered", Callable(this, "enter_visual_event"));
    visual_collision_area->connect("body_exited", Callable(this, "exit_visual_event"));
}
NodePath  GD3Dtd_character::get_visual_collision_area_node() const { return visual_collision_area_node; }

Vector3 GD3Dtd_character::get_lookat_position() const { return lookat_position; }
Node3D* GD3Dtd_character::get_aim_node()const { return old_aim_node; }
Vector3 GD3Dtd_character::get_character_forward() const { return character_forward; }
Vector3 GD3Dtd_character::get_character_direction() const { return character_direction; }
Vector3 GD3Dtd_character::get_character_direction_plane() const { return character_direction_plane; }
float GD3Dtd_character::get_character_movement_dot() const { return character_movement_dot; }

void GD3Dtd_character::set_mouse_sensitivity(const float set) { mouse_sensitivity = set; }
float GD3Dtd_character::get_mouse_sensitivity() const { return mouse_sensitivity; }
void GD3Dtd_character::set_turn_speed(const float set) { turn_speed = set; }
float GD3Dtd_character::get_turn_speed() const { return turn_speed; }
void GD3Dtd_character::set_instant_turn_aiming(const bool set) { instant_turn_aiming = set; }
bool GD3Dtd_character::get_instant_turn_aiming() const { return instant_turn_aiming; }
void GD3Dtd_character::set_invert_camera_movement(const bool set) { invert_camera_movement = set; }
bool GD3Dtd_character::get_invert_camera_movement() const { return invert_camera_movement; }
void GD3Dtd_character::set_camera_boon(const Vector3& set) { camera_boon = set; }
Vector3 GD3Dtd_character::get_camera_boon() const { return camera_boon; }
void GD3Dtd_character::set_camera_predict(const float set) { camera_predict = set; }
float GD3Dtd_character::get_camera_predict() const { return camera_predict; }
void GD3Dtd_character::set_camera_predict_speed(const float set) { camera_predict_speed = set; }
float GD3Dtd_character::get_camera_predict_speed() const { return camera_predict_speed; }
void GD3Dtd_character::set_aim_collision_mask(const uint32_t set) { aim_collision_mask = set; }
uint32_t GD3Dtd_character::get_aim_collision_mask() const { return aim_collision_mask; }

#pragma endregion Getters_and_setters