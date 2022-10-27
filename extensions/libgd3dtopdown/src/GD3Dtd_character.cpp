#include "GD3Dtd_character.hpp"

GD3Dtd_character::GD3Dtd_character()
{
    last_speed =0;
}
GD3Dtd_character::~GD3Dtd_character()
{
}

#define GETSET_GD3D(FUNC) ClassDB::bind_method(D_METHOD("get_"#FUNC), &GD3Dtd_character::get_##FUNC);\
                        ClassDB::bind_method(D_METHOD("set_"#FUNC, #FUNC), &GD3Dtd_character::set_##FUNC)
#define ADDPROP_GD3D(PROP,TYPE) ADD_PROPERTY(PropertyInfo(Variant::##TYPE, #PROP), "set_"#PROP, "get_"#PROP)
void GD3Dtd_character::_bind_methods()
{
  
    ClassDB::bind_method(D_METHOD("character_init"), &GD3Dtd_character::character_init);
    ClassDB::bind_method(D_METHOD("rotate_camera", "rotation_Vector2"), &GD3Dtd_character::rotate_camera);
    ClassDB::bind_method(D_METHOD("rotate_camera_aimlock","rotation_Vector2"), &GD3Dtd_character::rotate_camera_aimlock);
    ClassDB::bind_method(D_METHOD("rotate_camera_mouse", "event"), &GD3Dtd_character::rotate_camera_mouse);
    ClassDB::bind_method(D_METHOD("rotate_camera_mouse_aimlock","event"), &GD3Dtd_character::rotate_camera_mouse_aimlock);
    ClassDB::bind_method(D_METHOD("character_move","delta","input_Vector2","is_aiming","is_sprinting"), &GD3Dtd_character::character_move);
   
    
    GETSET_GD3D(mouse_sensitivity);
    GETSET_GD3D(walk_speed);
    GETSET_GD3D(sprint_speed);
    GETSET_GD3D(turn_speed);
    GETSET_GD3D(instant_turn_aiming);
    GETSET_GD3D(camera_node_path);
    GETSET_GD3D(invert_camera_movement);
    GETSET_GD3D(camera_boon);
    GETSET_GD3D(camera_predict);
    GETSET_GD3D(camera_predict_speed);

    GETSET_GD3D(interiors_collision_box_size);
    GETSET_GD3D(interiors_collision_box_centre);
    GETSET_GD3D(interiors_collision_mask);
    GETSET_GD3D(visual_collision_mask);
    GETSET_GD3D(aim_collision_mask);

    ClassDB::bind_method(D_METHOD("get_last_speed"), &GD3Dtd_character::get_last_speed);
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

    ADDPROP_GD3D(mouse_sensitivity, FLOAT);
    ADDPROP_GD3D(walk_speed, FLOAT);
    ADDPROP_GD3D(sprint_speed, FLOAT);
    ADDPROP_GD3D(turn_speed, FLOAT);
    ADDPROP_GD3D(instant_turn_aiming, BOOL);
    ADDPROP_GD3D(camera_node_path, NODE_PATH);
    ADDPROP_GD3D(invert_camera_movement, BOOL);
    ADDPROP_GD3D(camera_boon, VECTOR3);
    ADDPROP_GD3D(camera_predict, FLOAT);
    ADDPROP_GD3D(camera_predict_speed, FLOAT);
    ADDPROP_GD3D(interiors_collision_box_size, VECTOR3);
    ADDPROP_GD3D(interiors_collision_box_centre, VECTOR3);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "interiors_collision_mask", PROPERTY_HINT_LAYERS_3D_PHYSICS), "set_interiors_collision_mask", "get_interiors_collision_mask");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "visual_collision_mask", PROPERTY_HINT_LAYERS_3D_PHYSICS), "set_visual_collision_mask", "get_visual_collision_mask");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "aim_collision_mask", PROPERTY_HINT_LAYERS_3D_PHYSICS), "set_aim_collision_mask", "get_aim_collision_mask");
}
#undef GETSET_GD3D
#undef ADDPROP_GD3D
//Initializer functions, get pointers to Input and project settings. Set gravity value
#define CHECKNULL_ERRGD3D(PTR) if( PTR == nullptr ){ _uninitialize() ; ERR_FAIL_V_MSG(false, "Could not obtain "#PTR" pointer, didnt initialize");}
bool GD3Dtd_character::_initialize()
{
    if (initialized) return false;

#pragma region Needed_pointers_and_child_node_instantiation
    //General pointers
    ProjectSettings* p_settings = ProjectSettings::get_singleton();
    ph_server = PhysicsServer3D::get_singleton();
    camera = Object::cast_to<Camera3D>(get_node<Node>(camera_node_path));

    //Area3D pointers
    visual_collision_area = memnew(Area3D);
    CollisionShape3D* visual_col_shp = memnew(CollisionShape3D);
    BoxShape3D* visual_box = memnew(BoxShape3D);
    
    interiors_collision_area = memnew(Area3D);
    CollisionShape3D* interiors_collision_col_shp = memnew(CollisionShape3D);
    BoxShape3D* interiors_collision_box = memnew(BoxShape3D);

    w3d = get_world_3d().ptr();
    //Null checks
    CHECKNULL_ERRGD3D(p_settings);
    CHECKNULL_ERRGD3D(ph_server);
    CHECKNULL_ERRGD3D(camera);
    CHECKNULL_ERRGD3D(w3d);
    CHECKNULL_ERRGD3D(visual_collision_area);
    CHECKNULL_ERRGD3D(visual_col_shp);
    CHECKNULL_ERRGD3D(visual_box);
    CHECKNULL_ERRGD3D(interiors_collision_area);
    CHECKNULL_ERRGD3D(interiors_collision_col_shp);
    CHECKNULL_ERRGD3D(interiors_collision_box);
   
#pragma endregion Needed_pointers_and_child_nodes

    
#pragma region Child_node_initialization
    //Visual obstacle Collision area initialization
    add_child(visual_collision_area);
    visual_collision_area->set_owner(this);
    visual_collision_area->add_child(visual_col_shp);
    visual_col_shp->set_owner(visual_collision_area);
    visual_col_shp->set_shape(visual_box);
    visual_collision_area->set_position(Vector3(0, 1, 0));
    visual_box->set_size(Vector3(0.1f, 0.1f, camera_boon.length()));
    visual_col_shp->set_position(Vector3(0, 0, -camera_boon.length() / 2.0));

    visual_collision_area->set_collision_layer(0);
    visual_collision_area->set_collision_mask(visual_collision_mask);
    visual_collision_area->set_monitoring(true);
    visual_collision_area->set_monitorable(true);

    visual_collision_area->connect("body_entered", Callable(this, "enter_visual_event"));
    visual_collision_area->connect("body_exited", Callable(this, "exit_visual_event"));

    //Interior Collision area initialization
    add_child(interiors_collision_area);
    interiors_collision_area->set_owner(this);
    interiors_collision_area->add_child(interiors_collision_col_shp);
    interiors_collision_col_shp->set_owner(interiors_collision_area);
    interiors_collision_col_shp->set_shape(interiors_collision_box);

    interiors_collision_box->set_size(interiors_collision_box_size);
    interiors_collision_area->set_position(interiors_collision_box_centre);

    interiors_collision_area->set_collision_layer(0);
    interiors_collision_area->set_collision_mask(visual_collision_mask);
    interiors_collision_area->set_monitoring(true);
    interiors_collision_area->set_monitorable(true);
#pragma endregion Child_node_initialization

    //Some warnings
#ifdef DEBUG

    if ((visual_collision_mask & aim_collision_mask) != 0)
    {
        WARN_PRINT("Visual_collision_mask and aim_collision_mask share layers this might lead to unexpected behavior");
    }
    if ((interiors_collision_mask & aim_collision_mask) != 0)
    {
        WARN_PRINT("interiors_collision_mask and aim_collision_mask share layers this might lead to unexpected behavior");
    }
#endif // DEBUG


    Variant grav = p_settings->get_setting("physics/3d/default_gravity");
    if (grav.get_type() == Variant::INT || grav.get_type() == Variant::FLOAT)
    {
        gravity = float(grav);
    }
    else
    {
        gravity = 9.8f;
#ifdef DEBUG
        WARN_PRINT("Could not obtain value for gravity from project settings, defaulting to 9.8");
#endif // DEBUG
    }


    interiors_collision_area->connect("area_entered", Callable(this, "enter_interior_event"));
    interiors_collision_area->connect("area_exited", Callable(this, "exit_interior_event"));

#ifdef DEBUG
    WARN_PRINT("Initialized GD3D with gravity: " + String::num_real(gravity));
#endif // DEBUG

    initialized = true;
    return initialized;
}
#undef CHECKNULL_ERRGD3D
void GD3Dtd_character::_uninitialize()
{
    if (visual_collision_area != nullptr)
    {
        if(visual_collision_area->is_connected("body_entered", Callable(this, "enter_visual_event")))
            visual_collision_area->disconnect("body_entered", Callable(this, "enter_visual_event"));
        if (visual_collision_area->is_connected("body_exited", Callable(this, "exit_visual_event")))
            visual_collision_area->disconnect("body_exited", Callable(this, "exit_visual_event"));
       memdelete(visual_collision_area);
    }
    if (interiors_collision_area != nullptr)
    {
        if(interiors_collision_area->is_connected("area_entered", Callable(this, "enter_interior_event")))
            interiors_collision_area->disconnect("area_entered", Callable(this, "enter_interior_event"));
        if (interiors_collision_area->is_connected("area_exited", Callable(this, "exit_interior_event")))
            interiors_collision_area->disconnect("area_exited", Callable(this, "exit_interior_event"));
        memdelete(interiors_collision_area);
    }

    visual_collision_area = nullptr;
    interiors_collision_area = nullptr;
    old_aim_node = nullptr;
    camera = nullptr;
    ph_server = nullptr;
    w3d = nullptr;
    initialized = false;
}
bool GD3Dtd_character::_is_initialized() const
{
    return initialized;
}

/*Overriden functions _ready, _inputand _physics_process seem to be called
on object instantiation in the editor but are not called during the game (Tested in debug and release,
will keep the preprocessor directives in case im missing something but if you intend to use it remove them as godot wont find the functions when exported if not)
this makes getting pointers to singletos or executing code impossible from them
in order to account for this, The functions are defined as {func}_handle and called from gdscript
see GD3Dtd_character_project/GD3Dtd_character_godot/scripts/GD3Dtd_character.gd
Calling Engine::get_singleton()->is_editor_hint() does not solve the issue as the functions are not ticking inside the Run
*/
void GD3Dtd_character::_ready()
{
   // if (Engine::get_singleton()->is_editor_hint()) return;
}

void GD3Dtd_character::_input(const Ref<InputEvent>& p_event)
{
   // if (Engine::get_singleton()->is_editor_hint()) return;
}
void GD3Dtd_character::_physics_process(double delta)
{
   // if (Engine::get_singleton()->is_editor_hint()) return;
}

void GD3Dtd_character::character_init()
{
    visual_collision_area = nullptr;
    interiors_collision_area = nullptr;
    old_aim_node = nullptr;
    camera = nullptr;
    ph_server = nullptr;
    w3d = nullptr;
    _initialize();
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
void GD3Dtd_character::character_move(double delta, const Vector2& input_dir, const bool aiming, const bool is_sprinting)
{
    if (!initialized) return;
    //Inputs and environment
    is_aiming = aiming;

    Vector3 vel = get_velocity();
    bool floored = is_on_floor();
    float speed = walk_speed;

    //Logic with inputs
    if (!floored)
    {
        vel.y -= gravity * delta;
    }

    if (is_sprinting && floored)
    {
        speed = sprint_speed;
    }

    Vector3 direction = (camera->get_transform().basis.xform(Vector3(input_dir.x, 0, input_dir.y))).normalized();

    Vector3 lookat_pos = get_position();
    Vector3 front_pos = -get_global_transform().get_basis().get_column(2);
    if (direction.is_zero_approx())
    {
        vel.x = Math::move_toward(vel.x, 0, speed);
        vel.z = Math::move_toward(vel.z, 0, speed);


        lookat_pos.x += front_pos.x;
        lookat_pos.z += front_pos.z;
    }
    else
    {
        
        vel.x = Math::lerp(vel.x, direction.x * speed, 0.25f);
        vel.z = Math::lerp(vel.z, direction.z * speed, 0.25f);
        //vel.z = direction.z * speed;
        lookat_pos.x += direction.x;
        lookat_pos.z += direction.z;
    }

    if (is_aiming)
    {
        Vector2 mouse_pos = get_viewport()->get_mouse_position();

        Ref<PhysicsRayQueryParameters3D> ray = PhysicsRayQueryParameters3D::create(
            camera->project_ray_origin(mouse_pos),
            camera->project_ray_normal(mouse_pos) * 1000, aim_collision_mask);

        Dictionary ray_dict = ph_server->space_get_direct_state(
            w3d->get_space())->intersect_ray(ray);
        if (ray_dict != Dictionary())
        {
            Vector3 pos = ray_dict["position"];
            lookat_pos.x = pos.x;
            lookat_pos.z = pos.z;

            Node3D* nd = cast_to<Node3D>(ray_dict["collider"]);
            handle_aim_node(nd);
        }
        else
        {
            handle_aim_node(nullptr);
        }
    }
    else
    {
        if (old_aim_node != nullptr)
        {
            //Deselect function here
            old_aim_node = nullptr;
        }
    }

    camera_follow_position = camera_follow_position.lerp(camera_predict * direction.length_squared()
        * front_pos + get_position(), camera_predict_speed * delta);

    //Setting the resulting logic to the character and camera nodes
    set_velocity(vel);
    move_and_slide();
    if(!is_aiming || !instant_turn_aiming)
    {
        lookat_position = lookat_position.lerp(lookat_pos, turn_speed);
    }
    else
    {
        lookat_position = lookat_pos;
    }
    look_at(Vector3(lookat_position.x,get_position().y,lookat_position.z));
    Vector3 cam_pos = camera_follow_position + camera_boon;
    camera->set_position(cam_pos);
    camera->look_at(camera_follow_position);
    visual_collision_area->look_at(cam_pos);

    //Character info
    Vector3 dir_norm = vel.normalized();
    last_speed = speed;
    character_forward = -get_global_transform().get_basis().get_column(2);
    character_direction = dir_norm;
    character_direction_plane =Vector3(dir_norm.x,get_position().y,dir_norm.z);
    character_movement_dot = character_forward.dot(character_direction_plane);

}
#pragma endregion Character_movement_function

#pragma region Character_interaction_functions
void GD3Dtd_character::handle_aim_node(Node3D* nd)
{
    if (old_aim_node == nd) return;
    if (old_aim_node != nullptr)
    {
        //cast_to
        //if(old_aim_node->get_class() == "selectable_class")
        //DeHighlight function old_aim_node_->
    }
    old_aim_node = nd;

    if (old_aim_node != nullptr)
    {
        //cast_to
        //if(old_aim_node->get_class() == "selectable_class")
        // 
        //Highlight_function
    }
}
void GD3Dtd_character::enter_visual_event(Object* body)
{
    GD3Dvisual_obstacle* ar = cast_to<GD3Dvisual_obstacle>(body); 
    if (ar != nullptr && ar->get_class() == "GD3Dvisual_obstacle")
        ar->obstacle_entered(aim_collision_mask);
}
void GD3Dtd_character::exit_visual_event(Object* body)
{
    GD3Dvisual_obstacle* ar = cast_to<GD3Dvisual_obstacle>(body);
    if (ar != nullptr && ar->get_class() == "GD3Dvisual_obstacle")
        ar->obstacle_exited(aim_collision_mask);
}
void GD3Dtd_character::enter_interior_event(Object* area)
{
    GD3Dinterior_area* in_area = cast_to<GD3Dinterior_area>(area);
    if (in_area != nullptr && in_area->get_class() == "GD3Dinterior_area") 
        in_area->on_enter_area(aim_collision_mask);
}
void GD3Dtd_character::exit_interior_event(Object* area)
{
    GD3Dinterior_area* in_area = cast_to<GD3Dinterior_area>(area);
    if (in_area != nullptr && in_area->get_class() == "GD3Dinterior_area")
        in_area->on_exit_area(aim_collision_mask);
}
#pragma endregion Character_interaction_functions
#pragma region Getters_and_setters
//Getters and setters
float GD3Dtd_character::get_last_speed() const
{
    return last_speed;
}
Vector3 GD3Dtd_character::get_lookat_position() const
{
    return lookat_position;
}
Node3D* GD3Dtd_character::get_aim_node()const
{
    return old_aim_node;
}

void GD3Dtd_character::set_visual_collision_mask(uint32_t p_mask)
{
    visual_collision_mask = p_mask;
    /*Null checks in setter and getter cause editor to crash
    if (visual_collision_area == nullptr) return;

    visual_collision_area->set_collision_mask(p_mask);
    ;*/
}
uint32_t GD3Dtd_character::get_visual_collision_mask() const
{
    uint32_t msk = visual_collision_mask;
    /*
    if (visual_collision_area == nullptr) return msk;

    msk = visual_collision_area->get_collision_mask();
     }*/
    return msk;
}

Vector3 GD3Dtd_character::get_character_forward() const { return character_forward; }
Vector3 GD3Dtd_character::get_character_direction() const { return character_direction; }
Vector3 GD3Dtd_character::get_character_direction_plane() const { return character_direction_plane; }
float GD3Dtd_character::get_character_movement_dot() const { return character_movement_dot; }

#define GETTERSETTER_GD3D(VAR,TYPE) void GD3Dtd_character::set_##VAR##(const TYPE##& set) { VAR = set;}\
                                            TYPE GD3Dtd_character::get_##VAR##() const {return VAR ;}
GETTERSETTER_GD3D(mouse_sensitivity, float);
GETTERSETTER_GD3D(walk_speed, float);
GETTERSETTER_GD3D(sprint_speed, float);
GETTERSETTER_GD3D(turn_speed, float);
GETTERSETTER_GD3D(instant_turn_aiming, bool);
GETTERSETTER_GD3D(camera_node_path, NodePath);
GETTERSETTER_GD3D(invert_camera_movement, bool);
GETTERSETTER_GD3D(camera_boon, Vector3);
GETTERSETTER_GD3D(camera_predict, float);
GETTERSETTER_GD3D(camera_predict_speed, float);
GETTERSETTER_GD3D(aim_collision_mask, uint32_t);
GETTERSETTER_GD3D(interiors_collision_mask, uint32_t);
GETTERSETTER_GD3D(interiors_collision_box_size, Vector3);
GETTERSETTER_GD3D(interiors_collision_box_centre, Vector3);

#undef GETTERSETTER_GD3D
#pragma endregion Getters_and_setters