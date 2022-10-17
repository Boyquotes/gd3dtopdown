#ifdef WIN32
#include <windows.h>
#endif
#ifndef GD3DTOPDOWN
#define GD3DTOPDOWN

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/global_constants.hpp>

#include <godot_cpp/classes/character_body3d.hpp>

#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/input_map.hpp>
#include <godot_cpp/classes/input.hpp>
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
class GD3Dtopdown : public CharacterBody3D {
    

    GDCLASS(GD3Dtopdown, CharacterBody3D);

    GD3Dtopdown();
    ~GD3Dtopdown();
protected:
    static void _bind_methods();
    
private:
   
    bool initialized;
    //Singletons
    Input* input;
    ProjectSettings* p_settings;
    PhysicsServer3D* ph_server;
    Ref<World3D> w3d;

    //Values
    float gravity = 9.8f;
    float mouse_sensitivity = 0.2f;
    float walk_speed = 5.0f;
    float sprint_speed = 10.0f;

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
    Node3D* old_aim_node;

//Visual obstacles
    Area3D* interiors_collision_area;

    uint32_t visual_collision_mask;
    Array visual_obstacle_rayexcludes = {};
    Area3D* visual_obstacle_collision_area;

public:

    virtual bool _is_initialized()  const ;
    virtual bool _initialize() ;
    virtual void _uninitialize() ;

    virtual void _ready() override;
    virtual void _input(const Ref<InputEvent>& p_event) override;
    virtual void _physics_process(double delta) override;
   
    
#if defined(DEBUG_ENABLED)
    void _ready_handle();
    void _physics_process_handle(double delta);
    void _input_handle(const Ref<InputEvent>& p_event);
#endif
   
    //Other functions
    void handle_aim_node(Node3D* nd);

    //Signals
    void enter_visual_obstacle_event(Variant body);
    void exit_visual_obstacle_event(Variant body);

    void enter_interior_event(Variant area);
    void exit_interior_event(Variant area);
    
    
    //Getters and setters
    void set_mouse_sensitivity(const float sen);
    void set_walk_speed(const float spd);
    void set_sprint_speed(const float spd);
    void set_camera_node_path(const NodePath& path);
    void set_camera_boon(const Vector3& pos);
    void set_camera_predict(const float pre);
    void set_camera_predict_speed(const float cps);
    void set_invert_camera_movement(const bool inv);
    void set_visual_collision_mask(uint32_t p_mask);

    float get_mouse_sensitivity() const;
    float get_walk_speed() const;
    float get_sprint_speed() const;
    Vector3 get_lookat_position() const;
    NodePath get_camera_node_path() const;
    Vector3 get_camera_boon() const;
    float get_camera_predict() const;
    float get_camera_predict_speed() const;
    bool get_invert_camera_movement() const;
    Node3D* get_aim_node()const;
    uint32_t get_visual_collision_mask() const;
  
};
#endif 