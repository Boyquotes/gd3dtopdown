#ifdef WIN32
#include <windows.h>
#endif
#ifndef GD3DTD_CHARACTER
#define GD3DTD_CHARACTER

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/global_constants.hpp>

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
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/node.hpp>

#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/shape3d.hpp>
#include <godot_cpp/classes/box_shape3d.hpp>

#include <godot_cpp/classes/physics_body3d.hpp>

#include "GD3Dvisual_obstacle.hpp"
#include "GD3Dinterior_area.hpp"

using namespace godot;

class GD3Dtd_character : public CharacterBody3D {


    GDCLASS(GD3Dtd_character, CharacterBody3D);

    GD3Dtd_character();
    ~GD3Dtd_character();
protected:
    static void _bind_methods();

private:

    bool initialized;
    //Singletons
    PhysicsServer3D* ph_server;
    Ref<World3D> w3d;

    //Values
    float gravity = 9.8f;
    float mouse_sensitivity = 0.2f;
    float walk_speed = 5.0f;
    float sprint_speed = 10.0f;
    float turn_speed = 0.5f;
    bool instant_turn_aiming;

    //Aiming and camera properties
    bool is_aiming;
    Vector3 lookat_position;
    NodePath camera_node_path;
    Camera3D* camera;
    Vector3 camera_boon = Vector3(0, 15, 10);
    float camera_predict = 1;
    float camera_predict_speed = 15;
    Vector3 camera_follow_position;
    bool invert_camera_movement;

    uint32_t aim_collision_mask = 0;
    Node3D* old_aim_node;

    //Visual obstacles
    Vector3 interiors_collision_box_size = Vector3(0.1f,0.1f,0.1f);
    Vector3 interiors_collision_box_centre = Vector3(0,1,0);
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

    virtual bool _is_initialized()  const;
    virtual bool _initialize();
    virtual void _uninitialize();

    virtual void _ready() override;
    virtual void _input(const Ref<InputEvent>& p_event) override;
    virtual void _physics_process(double delta) override;


    void character_init();
    void character_move_full(double delta, const Vector2& input_dir, const bool aiming, const bool is_sprinting);
    void character_move_aim(double delta, const  Vector2& input_dir, const bool is_sprinting);
    void character_move_no_aim(double delta, const  Vector2& input_dir, const bool is_sprintin);
    void rotate_camera_mouse(const Ref<InputEvent>& p_event);
    void rotate_camera_mouse_aimlock(const Ref<InputEvent>& p_event);
    void rotate_camera(const Vector2& motion);
    void rotate_camera_aimlock(const Vector2& motion);

    //Other functions
    void handle_aim_node(Node3D* nd);

    //Signal Calls
    void enter_visual_event(Variant body);
    void exit_visual_event(Variant body);

    void enter_interior_event(Variant area);
    void exit_interior_event(Variant area);

    //Getters and setters

    Vector3 get_character_forward() const;
    Vector3 get_character_direction() const;
    Vector3 get_character_direction_plane() const;
    float get_character_movement_dot() const;
#define GETTERSETTER_GD3D(VAR,TYPE) void set_##VAR##(const TYPE##& set);\
                                            TYPE get_##VAR##() const
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
    GETTERSETTER_GD3D(interiors_collision_mask, uint32_t);
    GETTERSETTER_GD3D(aim_collision_mask, uint32_t);
    GETTERSETTER_GD3D(interiors_collision_box_size, Vector3);
    GETTERSETTER_GD3D(interiors_collision_box_centre, Vector3);
#undef GETTERSETTER_GD3D

    uint32_t get_visual_collision_mask() const;
    void set_visual_collision_mask(uint32_t p_mask);

    Vector3 get_lookat_position() const;
    Node3D* get_aim_node()const;

};
#endif 