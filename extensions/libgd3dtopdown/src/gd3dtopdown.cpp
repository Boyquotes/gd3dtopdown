#include "gd3ddtopdown.hpp"

#include <godot_cpp/classes/ref.hpp>

GD3Dtopdown::~GD3Dtopdown()
{
    _uninitialize();
}

void GD3Dtopdown::_bind_methods()
{
   // ClassDB::bind_method(D_METHOD("_input", "event"), &GD3Dtopdown::_input);

    
    ClassDB::bind_method(D_METHOD("get_player_position"), &GD3Dtopdown::get_player_position);
    ClassDB::bind_method(D_METHOD("set_player_position", "player_position"), &GD3Dtopdown::set_player_position);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "player_position"), "set_player_position", "get_player_position");

    ClassDB::bind_method(D_METHOD("get_lookat_position"), &GD3Dtopdown::get_lookat_position);
    ClassDB::bind_method(D_METHOD("set_lookat_position", "lookat_position"), &GD3Dtopdown::set_lookat_position);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "lookat_position"), "set_lookat_position", "get_lookat_position");

    ClassDB::bind_method(D_METHOD("get_camera_transform"), &GD3Dtopdown::get_camera_transform);
    ClassDB::bind_method(D_METHOD("set_camera_transform", "camera_transform"), &GD3Dtopdown::set_camera_transform);
 
    ClassDB::bind_method(D_METHOD("move_player_with_camera_direction"), &GD3Dtopdown::move_player_with_camera_direction);

    
}

void GD3Dtopdown::set_player_position(const Vector3& pos)
{
    player_position = pos;
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
Vector3 GD3Dtopdown::get_player_position() const
{
    
    return player_position;
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
        input = Input::get_singleton();
        if (input == nullptr)
        {
            ERR_FAIL_V_MSG(false,"Couldn't obtain Input singleton, didnt initialize");
        }
        
        initialized = true;
        WARN_PRINT("Initialized GD3D");
    }
    return initialized;
}
void GD3Dtopdown::_uninitialize()
{
    if (initialized)
    {
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
void GD3Dtopdown::_input(const Ref<InputEvent>& p_event)
{
    WARN_PRINT("InputFunctionCalledRef");
    Ref<InputEventMouseMotion> m = p_event;
    if (m.is_valid())
    {
        WARN_PRINT("moving");
    }

    return;
}

Vector3 GD3Dtopdown::move_player_with_camera_direction(const Vector2& inputvector)
{
   // Forward vector
    -(camera_transform.basis.get_axis(2));

    return -camera_transform.basis.get_axis(2);
    move_and_slide();
}
