#ifndef GD3DTD_CHARACTER
#define GD3DTD_CHARACTER
#ifdef WIN32
#include <windows.h>
#endif

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/character_body3d.hpp>

#include <godot_cpp/classes/ref.hpp>

#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/viewport.hpp>

#include <godot_cpp/classes/world3d.hpp>
#include <godot_cpp/classes/physics_server3d.hpp>
#include <godot_cpp/classes/physics_ray_query_parameters3d.hpp>
#include <godot_cpp/classes/physics_direct_space_state3d.hpp>


using namespace godot;

class GD3Dtd_character : public CharacterBody3D {


    GDCLASS(GD3Dtd_character, CharacterBody3D);

   
protected:
    static void _bind_methods();
    void _notification(int p_what);

private:

    bool initialized;
    //Values
    float gravity = 9.8f;

    Camera3D* p_camera;

    Vector3 lookat_position;
    Vector3 camera_boon;
    Vector3 cam_height;
    Vector3 camera_follow_position;

    uint32_t aim_collision_mask = 0;
    Object* old_aim_node;

public:
#include "GD3Dhelpers.hpp"

public:
    PackedStringArray _get_configuration_warnings() const override;
    void on_parented(Variant v_child);
    void on_unparented(Variant v_child);
    //unused unless exposed by godot-cpp
    virtual void add_child_notify(Node* p_child);
    virtual void remove_child_notify(Node* p_child);

    void character_move(const double delta, const Vector2& input_dir, const bool aiming,
            const float speed,const float move_lerp, const float turn_speed = 0.5f,
            const float camera_predict = 1.0f, const float camera_predict_speed = 15.0f);

    void rotate_camera_mouse(const Ref<InputEvent>& p_event, const float mouse_sensitivity = 0.2f, const bool is_inverted = true);
    void rotate_camera(const Vector2& motion, const float mouse_sensitivity = 0.2f, const bool is_inverted = true);

    //Other functions
    void handle_aim(const bool aiming,Vector3& lookat_pos);
    void handle_aim_node(Object* nd);

    void set_aim_collision_mask(const uint32_t set); 
    uint32_t get_aim_collision_mask() const; 

};
#endif 