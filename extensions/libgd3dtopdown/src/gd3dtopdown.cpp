#include "gd3ddtopdown.hpp"

GD3Dtopdown::GD3Dtopdown()
{
    initialized = false;
    
    //Register input actions if they are not registerd beforehand
    InputMap* input_map = InputMap::get_singleton();
    
    if(!input_map->has_action("jump"))         input_map->add_action("jump");
    if(!input_map->has_action("sprint"))       input_map->add_action("sprint");
    if(!input_map->has_action("move_left"))    input_map->add_action("move_left");
    if(!input_map->has_action("move_right"))   input_map->add_action("move_right");
    if(!input_map->has_action("move_forward")) input_map->add_action("move_forward");
    if(!input_map->has_action("move_back"))    input_map->add_action("move_back");
    if(!input_map->has_action("aim"))    input_map->add_action("aim");
}
GD3Dtopdown::~GD3Dtopdown()
{
    _uninitialize();
}

void GD3Dtopdown::_bind_methods()
{
#if  defined(DEBUG_ENABLED) 
    ClassDB::bind_method(D_METHOD("_ready_handle"), &GD3Dtopdown::_ready_handle);
    ClassDB::bind_method(D_METHOD("_input_handle"), &GD3Dtopdown::_input_handle);
    ClassDB::bind_method(D_METHOD("_physics_process_handle"), &GD3Dtopdown::_physics_process_handle);
#endif

    ClassDB::bind_method(D_METHOD("get_mouse_sensitivity"), &GD3Dtopdown::get_mouse_sensitivity);
    ClassDB::bind_method(D_METHOD("set_mouse_sensitivity", "mouse_sensitivity"), &GD3Dtopdown::set_mouse_sensitivity);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "mouse_sensitivity"), "set_mouse_sensitivity", "get_mouse_sensitivity");

    ClassDB::bind_method(D_METHOD("get_walk_speed"), &GD3Dtopdown::get_walk_speed);
    ClassDB::bind_method(D_METHOD("set_walk_speed", "walk_speed"), &GD3Dtopdown::set_walk_speed);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "walk_speed"), "set_walk_speed", "get_walk_speed");

    ClassDB::bind_method(D_METHOD("get_sprint_speed"), &GD3Dtopdown::get_sprint_speed);
    ClassDB::bind_method(D_METHOD("set_sprint_speed", "sprint_speed"), &GD3Dtopdown::set_sprint_speed);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "sprint_speed"), "set_sprint_speed", "get_sprint_speed");

    ClassDB::bind_method(D_METHOD("get_player_jump_velocity"), &GD3Dtopdown::get_player_jump_velocity);
    ClassDB::bind_method(D_METHOD("set_player_jump_velocity", "player_jump_velocity"), &GD3Dtopdown::set_player_jump_velocity);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "player_jump_velocity"), "set_player_jump_velocity", "get_player_jump_velocity");
    

    ClassDB::bind_method(D_METHOD("get_lookat_position"), &GD3Dtopdown::get_lookat_position);
    ClassDB::bind_method(D_METHOD("set_lookat_position", "lookat_position"), &GD3Dtopdown::set_lookat_position);
    
    ClassDB::bind_method(D_METHOD("get_camera_node_path"), &GD3Dtopdown::get_camera_node_path);
    ClassDB::bind_method(D_METHOD("set_camera_node_path", "camera_node_path"), &GD3Dtopdown::set_camera_node_path);
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "camera_node_path"), "set_camera_node_path", "get_camera_node_path");

    ClassDB::bind_method(D_METHOD("get_camera_boon"), &GD3Dtopdown::get_camera_boon);
    ClassDB::bind_method(D_METHOD("set_camera_boon", "camera_boon"), &GD3Dtopdown::set_camera_boon);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "camera_boon"), "set_camera_boon", "get_camera_boon");

    ClassDB::bind_method(D_METHOD("get_camera_predict"), &GD3Dtopdown::get_camera_predict);
    ClassDB::bind_method(D_METHOD("set_camera_predict", "camera_predict"), &GD3Dtopdown::set_camera_predict);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "camera_predict"), "set_camera_predict", "get_camera_predict");

    ClassDB::bind_method(D_METHOD("get_camera_predict_speed"), &GD3Dtopdown::get_camera_predict_speed);
    ClassDB::bind_method(D_METHOD("set_camera_predict_speed", "camera_predict_speed"), &GD3Dtopdown::set_camera_predict_speed);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "camera_predict_speed"), "set_camera_predict_speed", "get_camera_predict_speed");
}

//Initializer functions, get pointers to Input and project settings. Set gravity value
bool GD3Dtopdown::_initialize()
{
    if (!initialized)
    {
        p_settings = ProjectSettings::get_singleton();
        input = Input::get_singleton();
        ph_server = PhysicsServer3D::get_singleton();

        if (p_settings == nullptr)
        {
            _uninitialize();
            ERR_FAIL_V_MSG(false, "Could not obtain ProjectSettings singleton, didnt initialize");
        }
        if (input == nullptr)
        {
            _uninitialize();
            ERR_FAIL_V_MSG(false, "Could not obtain Input singleton, didnt initialize");
        }
        if (ph_server== nullptr)
        {
            _uninitialize();
            ERR_FAIL_V_MSG(false, "Could not obtain Physics Server singleton, didnt initialize");
        }
        w3d = get_world_3d();
        if (!w3d.is_valid()) 
        {
            _uninitialize();
            ERR_FAIL_V_MSG(false, "Could not obtain World 3d reference, didnt initialize");
        }
        Variant grav = p_settings->get_setting("physics/3d/default_gravity");
        if (grav.get_type() == Variant::INT || grav.get_type() == Variant::FLOAT)
        {
            gravity = float(grav);
        }
        else
        {
            gravity = 9.8f;
            WARN_PRINT("Could not obtain value for gravity from project settings, defaulting to 9.8");
        }
        camera = Object::cast_to<Camera3D>(get_node<Node>(camera_node_path));
        if (camera == nullptr)
        {
            _uninitialize();
            ERR_FAIL_V_MSG(false, "Could not obtain reference to camera didnt initialize");
        }
        initialized = true;
        WARN_PRINT(DEBUG_STR("Initialized GD3D with gravity: " + String::num_real(gravity)));

    }
    return initialized;
}
void GD3Dtopdown::_uninitialize()
{
    if (initialized)
    {
        p_settings = nullptr;
        input = nullptr;
        camera = nullptr;

        initialized = false;
    }
    return;
}
bool GD3Dtopdown::_is_initialized() const
{
    return initialized;
}

/*Overriden functions _ready, _inputand _physics_process seem to be called
on object instantiation in the editor but are not called during the game (Tested in debug and release,
will keep the preprocessor directives in case im missing something but if you intend to use it remove them as godot wont find the functions when exported if not)
this makes getting pointers to singletos or executing code impossible from them
in order to account for this, The functions are defined as {func}_handle and called from gdscript
see gd3dtopdown_project/gd3dtopdown_godot/scripts/gd3dtopdown.gd
*/
void GD3Dtopdown::_ready()
{
#if defined(DEBUG_ENABLED)
    WARN_PRINT_ONCE("Ready function called");
}
void GD3Dtopdown::_ready_handle()
{
    WARN_PRINT_ONCE("Ready handle function called");
#endif
    _initialize();
}

void GD3Dtopdown::_input(const Ref<InputEvent>& p_event)
{
    
#if defined(DEBUG_ENABLED)
    WARN_PRINT_ONCE("Input function called");
}
void GD3Dtopdown::_input_handle(const Ref<InputEvent>& p_event)
{
    WARN_PRINT_ONCE("Input handle function called");
#endif
   
    Ref<InputEventMouseMotion> m = p_event;
    if (m.is_valid() && !is_aiming)
    {
        Vector2 motion = m->get_relative();
        camera_boon.rotate( Vector3(0,1,0), Math::deg_to_rad(motion.x) * mouse_sensitivity);
        camera->set_position(camera_follow_position + camera_boon);
        camera->look_at(camera_follow_position);
    }
    return;
}

void GD3Dtopdown::_physics_process(double delta)
{
#if defined(DEBUG_ENABLED) 
    WARN_PRINT_ONCE("Physics process called");
}
void GD3Dtopdown::_physics_process_handle(double delta)
{
    WARN_PRINT_ONCE("Physics handle process called");
#endif
    //Inputs and environment
    is_aiming = input->is_action_pressed("aim");
    Vector2 input_dir = input->get_vector("move_left", "move_right", "move_forward", "move_back");
    bool is_jumping = input->is_action_pressed("jump");
    bool is_sprinting = input->is_action_pressed("sprint");
    Vector3 vel = get_velocity();
    bool floored = is_on_floor();
    float speed = walk_speed;

    //Logic with inputs
    if (!floored)
    {
        vel.y -= gravity * delta;
    }

    if (floored && is_jumping )
    {
        vel.y = player_jump_velocity;
    }

    if (is_sprinting && floored)
    {
        speed = sprint_speed;
    }

    Vector3 direction = (camera->get_transform().basis.xform(Vector3(input_dir.x, 0, input_dir.y))).normalized();
    
    lookat_position = get_position();
    if (direction.is_zero_approx())
    {
        vel.x = Math::move_toward(vel.x, 0, speed);
        vel.z = Math::move_toward(vel.z, 0, speed);
       
        Vector3 front_pos = get_global_transform().get_basis().get_column(2);
        lookat_position.x -=  front_pos.x;
        lookat_position.z -=  front_pos.z;
    }
    else
    {
        vel.x = direction.x * speed;
        vel.z = direction.z * speed;
        lookat_position.x += direction.x;
        lookat_position.z += direction.z;
    }

    if (is_aiming)
    {
        Vector2 mouse_pos = get_viewport()->get_mouse_position();

        Ref<PhysicsRayQueryParameters3D> ray = PhysicsRayQueryParameters3D::create(
                                            camera->project_ray_origin(mouse_pos), 
                                            camera->project_ray_normal(mouse_pos) * 1000);

        Dictionary ray_dict = ph_server->space_get_direct_state(
                                            w3d->get_space())->intersect_ray(ray);

        if(ray_dict.has("position"))
        {
            Vector3 pos = ray_dict["position"];
            lookat_position.x = pos.x;
            lookat_position.z = pos.z;
        }
        if (ray_dict.has("collider"))
        {
            Object* aim_obj = ray_dict["collider"];
            handle_collider(aim_obj);
        }
        /* No use yet but useful reference
        if(ray_dict.has("normal"))Vector3 norm = ray_dict["normal"];
        if (ray_dict.has("collider_id")) int obj_id = ray_dict["collider_id"];
        if (ray_dict.has("shape")) int shape = ray_dict["shape"];
        if (ray_dict.has("rid"))RID rid = ray_dict["rid"];
        */

    }
    
    camera_follow_position = camera_follow_position.lerp(camera_predict * direction + get_position(), camera_predict_speed * delta);
    
    //Setting the resulting logic to the character and camera nodes
    set_velocity(vel);
    move_and_slide();
    look_at(lookat_position);
    
    camera->set_position(camera_follow_position + camera_boon);
    camera->look_at(camera_follow_position);
}

//Other functions
void GD3Dtopdown::handle_collider(Object* obj)
{
    if (obj == old_aim_obj) return;
    old_aim_obj = obj;
    WARN_PRINT(obj->get_class());
}

//Setters and getters
void GD3Dtopdown::set_mouse_sensitivity(const float sen)
{
    mouse_sensitivity = sen;
}
void GD3Dtopdown::set_walk_speed(const float spd)
{
    walk_speed = spd;
}
void GD3Dtopdown::set_sprint_speed(const float spd)
{
    sprint_speed = spd;
}
void GD3Dtopdown::set_player_jump_velocity(const float vel)
{
    player_jump_velocity = vel;
}
void GD3Dtopdown::set_lookat_position(const Vector3& pos)
{
    lookat_position = pos;
}
void GD3Dtopdown::set_camera_node_path(const NodePath& path)
{
   camera_node_path = path;
}
void GD3Dtopdown::set_camera_boon(const Vector3& pos)
{
    camera_boon = pos;
}
void GD3Dtopdown::set_camera_predict(const float pre)
{
    camera_predict = pre;
}
void GD3Dtopdown::set_camera_predict_speed(const float cps)
{
    camera_predict_speed = cps;
}


float GD3Dtopdown::get_mouse_sensitivity() const
{
    return mouse_sensitivity;
}
float GD3Dtopdown::get_walk_speed() const
{
    return walk_speed;
}
float GD3Dtopdown::get_sprint_speed() const
{
    return sprint_speed;
}
float GD3Dtopdown::get_player_jump_velocity() const
{
    return player_jump_velocity;
}
Vector3 GD3Dtopdown::get_lookat_position() const
{
    return lookat_position;
}
NodePath GD3Dtopdown::get_camera_node_path() const
{
    return camera_node_path;
}
Vector3 GD3Dtopdown::get_camera_boon() const
{
    return camera_boon;
}
float GD3Dtopdown::get_camera_predict() const
{
    return camera_predict;
}

float GD3Dtopdown::get_camera_predict_speed() const
{
    return camera_predict_speed;
}

