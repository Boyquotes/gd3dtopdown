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
}
GD3Dtopdown::~GD3Dtopdown()
{
    _uninitialize();
}

void GD3Dtopdown::_bind_methods()
{
#if  defined(DEBUG_ENABLED) || defined(TEST_FUNCTIONS)
    ClassDB::bind_method(D_METHOD("_ready_handle"), &GD3Dtopdown::_ready_handle);
    ClassDB::bind_method(D_METHOD("_input_handle"), &GD3Dtopdown::_input_handle);
    ClassDB::bind_method(D_METHOD("_physics_process_handle"), &GD3Dtopdown::_physics_process_handle);
#endif

    ClassDB::bind_method(D_METHOD("get_mouse_sensitivity"), &GD3Dtopdown::get_mouse_sensitivity);
    ClassDB::bind_method(D_METHOD("set_mouse_sensitivity", "mouse_sensitivity"), &GD3Dtopdown::set_mouse_sensitivity);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "mouse_sensitivity"), "set_mouse_sensitivity", "get_mouse_sensitivity");

    ClassDB::bind_method(D_METHOD("get_player_speed"), &GD3Dtopdown::get_player_speed);
    ClassDB::bind_method(D_METHOD("set_player_speed", "player_speed"), &GD3Dtopdown::set_player_speed);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "player_speed"), "set_player_speed", "get_player_speed");

    ClassDB::bind_method(D_METHOD("get_player_jump_velocity"), &GD3Dtopdown::get_player_jump_velocity);
    ClassDB::bind_method(D_METHOD("set_player_jump_velocity", "player_jump_velocity"), &GD3Dtopdown::set_player_jump_velocity);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "player_jump_velocity"), "set_player_jump_velocity", "get_player_jump_velocity");
    

    ClassDB::bind_method(D_METHOD("get_lookat_position"), &GD3Dtopdown::get_lookat_position);
    ClassDB::bind_method(D_METHOD("set_lookat_position", "lookat_position"), &GD3Dtopdown::set_lookat_position);
    
    ClassDB::bind_method(D_METHOD("get_camera_node_path"), &GD3Dtopdown::get_camera_node_path);
    ClassDB::bind_method(D_METHOD("set_camera_node_path", "camera_node"), &GD3Dtopdown::set_camera_node_path);
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "camera_node_path"), "set_camera_node_path", "get_camera_node_path");
}

//Initializer functions, get pointers to Input and project settings. Set gravity value
bool GD3Dtopdown::_initialize()
{
    if (!initialized)
    {
        p_settings = ProjectSettings::get_singleton();
        input = Input::get_singleton();

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
#if defined(DEBUG_ENABLED) || defined(TEST_FUNCTIONS)
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
    
#if defined(DEBUG_ENABLED) || defined(TEST_FUNCTIONS)
    WARN_PRINT_ONCE("Input function called");
}
void GD3Dtopdown::_input_handle(const Ref<InputEvent>& p_event)
{
    WARN_PRINT_ONCE("Input handle function called");
#endif
   
    Ref<InputEventMouseMotion> m = p_event;
    if (m.is_valid())
    {
        Vector2 motion = m->get_relative();
        Vector3 rot = Vector3(0,Math::deg2rad(motion.x) * mouse_sensitivity,0);
        camera->set_rotation(camera->get_rotation() + rot);
    }
    return;
}

void GD3Dtopdown::_physics_process(double delta)
{
#if defined(DEBUG_ENABLED) || defined(TEST_FUNCTIONS)
    WARN_PRINT_ONCE("Physics process called");
}
void GD3Dtopdown::_physics_process_handle(double delta)
{
    WARN_PRINT_ONCE("Physics handle process called");
#endif
    Vector3 vel = get_velocity();

    bool floored = is_on_floor();
    float speed = player_speed;
    if (!floored)
    {
        vel.y -= gravity * delta;
    }

    if (floored && input->is_action_pressed("jump"))
    {
        vel.y = player_jump_velocity;
    }

    if (input->is_action_pressed("sprint"))
    {
        speed *= 2.0f;
    }

    Vector2 input_dir = input->get_vector("move_left", "move_right", "move_forward", "move_back");

    Vector3 direction = (get_transform().basis.xform(Vector3(input_dir.x, 0, input_dir.y))).normalized();

    if (direction.is_zero_approx())
    {
        vel.x = Math::move_toward(vel.x, 0, speed);
        vel.z = Math::move_toward(vel.z, 0, speed);
    }
    else
    {
        vel.x = direction.x * speed;
        vel.z = direction.z * speed;
    }
    set_velocity(vel);

    move_and_slide();
}

/*
Setters and getters
*/
void GD3Dtopdown::set_mouse_sensitivity(const float sen)
{
    mouse_sensitivity = sen;
}
void GD3Dtopdown::set_player_speed(const float spd)
{
    player_speed = spd;
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

float GD3Dtopdown::get_mouse_sensitivity() const
{
    return mouse_sensitivity;
}
float GD3Dtopdown::get_player_speed() const
{
    return player_speed;
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
