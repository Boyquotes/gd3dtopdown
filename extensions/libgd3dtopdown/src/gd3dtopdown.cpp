#include "gd3ddtopdown.hpp"

//#include <godot_cpp/classes/ref.hpp>
GD3Dtopdown::GD3Dtopdown()
{
    initialized = false;

}

GD3Dtopdown::~GD3Dtopdown()
{
    _uninitialize();
}

void GD3Dtopdown::_bind_methods()
{
    
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
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "lookat_position"), "set_lookat_position", "get_lookat_position");

    ClassDB::bind_method(D_METHOD("get_camera_transform"), &GD3Dtopdown::get_camera_transform);
    ClassDB::bind_method(D_METHOD("set_camera_transform", "camera_transform"), &GD3Dtopdown::set_camera_transform);
 
  //  ClassDB::bind_method(D_METHOD("move_player_with_camera_direction"), &GD3Dtopdown::move_player_with_camera_direction);
    //Register of input_handle
    ClassDB::bind_method(D_METHOD("_input_handle"), &GD3Dtopdown::_input_handle);
    
}

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
void GD3Dtopdown::set_camera_transform(const Transform3D& transform)
{
    //ERR_FAIL_MSG("Couldn't obtain CameraNode3D");
    
    camera_transform = transform;
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
Transform3D GD3Dtopdown::get_camera_transform() const
{
    return camera_transform;
}

bool GD3Dtopdown::_is_initialized() const
{
    return initialized;
}
bool GD3Dtopdown::_initialize()
{
    if (!initialized)
    {
        p_settings = ProjectSettings::get_singleton();
        input = Input::get_singleton();

        if (p_settings == nullptr)
        {
            _uninitialize();
            ERR_FAIL_V_MSG(false, "Couldn't obtain ProjectSettings singleton, didnt initialize");
        }
        if (input == nullptr)
        {
            _uninitialize();
            ERR_FAIL_V_MSG(false, "Couldn't obtain Input singleton, didnt initialize");
        }
        
        Variant grav = p_settings->get_setting("physics/3d/default_gravity");
        if (grav.get_type() == Variant::INT || grav.get_type() == Variant::FLOAT)
        {
            gravity = float(grav);
           
        }else
        {
            
            WARN_PRINT("Could not obtain value for gravity from project settings, defaulting to 9.8");
        }
        gravity = 9.8f;
        WARN_PRINT("Could not obtain value for gravity from project settings, defaulting to 9.8");
        initialized = true;
        WARN_PRINT("Initialized GD3D");
    }
    return initialized;
}
void GD3Dtopdown::_uninitialize()
{
    if (initialized)
    {
        p_settings = nullptr;
        input = nullptr;
        initialized = false;
    }
    return;
}

void GD3Dtopdown::_ready()
{
    _initialize();
    WARN_PRINT("Ready function executed");
}

void GD3Dtopdown::_input(const Ref<InputEvent>& p_event)
{
    //Not being captured void GD3Dtopdown::_input(Variant p_event) neither
    // use _input handle instead (look at gd3dtopdown_godot/scripts/gd3dtopdown.gd as a temporary workaround
    WARN_PRINT_ONCE("InputFunctionCalledRef");
}
void GD3Dtopdown::_input_handle(const Ref<InputEvent>& p_event)
{
    //Temporary workaround for handling input events

    Ref<InputEventMouseMotion> m = p_event;
    if (m.is_valid())
    {
        Vector2 motion = m->get_relative();
        rotate(Vector3(0, 1, 0), Math::deg2rad(motion.x) * mouse_sensitivity);
    }
    return;
}

void GD3Dtopdown::_physics_process(double delta)
{
    WARN_PRINT_ONCE("Physics process running");
    Vector3 vel = get_velocity();
    bool floored = is_on_floor();
    float speed = player_speed;

    if (!floored)
    {
        vel.y -= gravity * delta;
    }
    
    if (floored && input->is_action_pressed("jump"))
    {
        WARN_PRINT("Jump");
        vel.y = player_jump_velocity;
    }
    
    if (input->is_action_pressed("sprint"))
    {
        speed *= 2.0f;
    }
   
    Vector2 input_dir = input->get_vector("move_left", "move_right", "move_forward", "move_back");

    Vector3 direction = (get_transform().basis.xform(Vector3(input_dir.x, 0, input_dir.y))).normalized();
   
    if(direction.is_zero_approx())
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
   
}
/*void GD3Dtopdown::_input(Variant p_event)
{
    WARN_PRINT("InputFunctionCalled");
    Ref<InputEventMouseMotion> m = p_event;
    if (m.is_valid())
    {
        WARN_PRINT("moving");
    }

    return;
}*/
