#ifdef WIN32
#include <windows.h>
#endif
#ifndef GD3DTD_CHARACTER
#define GD3DTD_CHARACTER

#include <godot_cpp/core/binder_common.hpp>

#include <godot_cpp/classes/character_body3d.hpp>

#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>

#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/viewport.hpp>

#include <godot_cpp/classes/world3d.hpp>
#include <godot_cpp/classes/physics_server3d.hpp>
#include <godot_cpp/classes/physics_ray_query_parameters3d.hpp>
#include <godot_cpp/classes/physics_direct_space_state3d.hpp>

#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/box_shape3d.hpp>

#include "GD3Dvisual_obstacle.hpp"
#include "GD3Dinterior_area.hpp"
#include "GD3Dselectable_node.hpp"

using namespace godot;

class GD3Dtd_character : public CharacterBody3D {


    GDCLASS(GD3Dtd_character, CharacterBody3D);

    GD3Dtd_character();
    ~GD3Dtd_character();
protected:
    static void _bind_methods();

private:

    bool initialized;

    //Values
    float gravity = 9.8f;
    float mouse_sensitivity = 0.2f;
    float turn_speed = 0.5f;
    bool instant_turn_aiming;

    //Aiming and camera properties
    bool is_aiming;
    Vector3 lookat_position;
    NodePath camera_node;
    Camera3D* camera;
    Vector3 camera_boon = Vector3(0, 15, 10);
    float camera_predict = 1;
    float camera_predict_speed = 15;
    Vector3 camera_follow_position;
    bool invert_camera_movement;

    uint32_t aim_collision_mask = 0;
    Node3D* old_aim_node;

    //Visual obstacles
    NodePath  interiors_collision_area_node;
    NodePath  visual_collision_area_node;
    Area3D* interiors_collision_area;
    uint32_t interiors_collision_mask = 0;
    uint32_t visual_collision_mask = 0;
    Area3D* visual_collision_area;

    //Displacement and orientation vectors
    Vector3 character_forward;
    Vector3 character_direction;
    Vector3 character_direction_plane;
    float character_movement_dot;

public:

    virtual void _ready() override;
    virtual void _input(const Ref<InputEvent>& p_event) override;
    virtual void _physics_process(double delta) override;

    void character_init();
    void character_uninit();

    void character_move(const double delta, const Vector2& input_dir, const bool aiming, const float speed,const float move_lerp);
    void rotate_camera_mouse(const Ref<InputEvent>& p_event);
    void rotate_camera_mouse_aimlock(const Ref<InputEvent>& p_event);
    void rotate_camera(const Vector2& motion);
    void rotate_camera_aimlock(const Vector2& motion);

    //Other functions
    void handle_aim(const bool aiming,Vector3& lookat_pos);
    void handle_aim_node(Node3D* nd);

    //Signal Calls
    void enter_visual_event(Object* body);
    void exit_visual_event(Object* body);
    void enter_interior_event(Object* area);
    void exit_interior_event(Object* area);

    //Getters and setters
    Vector3 get_character_forward() const;
    Vector3 get_character_direction() const;
    Vector3 get_character_direction_plane() const;
    float get_character_movement_dot() const;
    Vector3 get_lookat_position() const;
    Node3D* get_aim_node() const;

    void set_visual_collision_mask(const uint32_t p_mask);
    uint32_t get_visual_collision_mask() const;
    void set_interiors_collision_mask(const uint32_t p_mask);
    uint32_t get_interiors_collision_mask() const;

    void set_mouse_sensitivity(const float set);
    float get_mouse_sensitivity() const;
    void set_turn_speed(const float set);
    float get_turn_speed() const;
    void set_instant_turn_aiming(const bool);
    bool get_instant_turn_aiming() const;
    void set_camera_node(const NodePath& set);
    NodePath get_camera_node() const;
    void set_interiors_collision_area_node(const NodePath& set);
    NodePath get_interiors_collision_area_node() const;
    void set_visual_collision_area_node(const NodePath& set);
    NodePath get_visual_collision_area_node() const;
    void set_invert_camera_movement(const bool);
    bool get_invert_camera_movement() const;
    void set_camera_boon(const Vector3& set);
    Vector3 get_camera_boon() const;
    void set_camera_predict(const float set);
    float get_camera_predict() const;
    void set_camera_predict_speed(const float set);
    float get_camera_predict_speed() const;
    void set_aim_collision_mask(const uint32_t set);
    uint32_t get_aim_collision_mask() const;
};
#endif 