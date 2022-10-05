#ifdef WIN32
#include <windows.h>
#endif

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/viewport.hpp>

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/input_event.hpp>

using namespace godot;

class GD3Dtopdown : public CharacterBody3D {
    GDCLASS(GD3Dtopdown, CharacterBody3D);

    ~GD3Dtopdown();
protected:
    static void _bind_methods();

private:
    bool initialized;
    
    Input* input;

    Vector3 player_position;
    Vector3 lookat_position;

    Transform3D camera_transform = Transform3D();

public:

    virtual bool _is_initialized()  const ;
    virtual bool _initialize() ;
    virtual void _uninitialize() ;


    void set_player_position(const Vector3 &pos);
    void set_lookat_position(const Vector3 &pos);
    void set_camera_transform(const Transform3D &transform);
    Vector3 get_player_position() const;
    Vector3 get_lookat_position() const;
    Transform3D get_camera_transform() const;

 //   void input(const Ref<InputEvent>& p_event) override;

    Vector3 move_player_with_camera_direction(const Vector2& dir);
    

    void _ready() override;
   
   // void _input(Variant p_event);
    void _input(const Ref<InputEvent>& p_event) override;
   
};