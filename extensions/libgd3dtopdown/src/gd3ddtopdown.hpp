#ifdef WIN32
#include <windows.h>
#endif

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


using namespace godot;

class GD3Dtopdown : public CharacterBody3D {
    GDCLASS(GD3Dtopdown, CharacterBody3D);

    GD3Dtopdown();
    ~GD3Dtopdown();
protected:
    static void _bind_methods();
    
private:
   
    //Singletons
    Input* input;
    ProjectSettings* p_settings;
    PhysicsServer3D* ph_server;
    Ref<World3D> w3d;

    bool initialized;

    float gravity = 9.8f;
    float mouse_sensitivity = 0.2f;
    float walk_speed = 5.0f;
    float sprint_speed = 10.0f;
    float player_jump_velocity = 4.5f;

//Aiming and camera properties
    bool is_aiming;
    Vector3 lookat_position;
    NodePath camera_node_path;
    Camera3D* camera;
    Vector3 camera_boon = Vector3(0, 10, 10);
    float camera_predict = 0;
    float camera_predict_speed = 15;
    Vector3 camera_follow_position;
    bool invert_camera_movement;

//Environment checks
    Array wall_rayexcludes = {};
    Array roof_rayexcludes = {};
    Array all_rayexcludes = {};
    Node3D* old_aim_node;
    Node3D* old_roof_node;
    Node3D* old_intersect_node;

    Area3D* roof_collision_area;
    Area3D* wall_collision_area;
    Ref<Shape3D> roof_detect_shape;
    Ref<Shape3D> wall_detect_shape;

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
    void check_for_roof() ;
    void check_camera_visibility() ;

    //Signals
    void enter_roof_event(Variant area);
    void enter_wall_event(Variant area);
    void exit_roof_event(Variant area);
    void exit_wall_event(Variant area);
    
    //Getters and setters
    void set_mouse_sensitivity(const float sen);
    void set_walk_speed(const float spd);
    void set_sprint_speed(const float spd);
    void set_player_jump_velocity(const float vel);
    void set_camera_node_path(const NodePath& path);
    void set_camera_boon(const Vector3& pos);
    void set_camera_predict(const float pre);
    void set_camera_predict_speed(const float cps);
    void set_invert_camera_movement(const bool inv);
    void set_roof_detect_shape(Ref<Shape3D> shape);
    void set_wall_detect_shape(Ref<Shape3D> shape);

    float get_mouse_sensitivity() const;
    float get_walk_speed() const;
    float get_sprint_speed() const;
    float get_player_jump_velocity() const;
    Vector3 get_lookat_position() const;
    NodePath get_camera_node_path() const;
    Vector3 get_camera_boon() const;
    float get_camera_predict() const;
    float get_camera_predict_speed() const;
    bool get_invert_camera_movement() const;
    Node3D* get_aim_node()const;
    Ref<Shape3D> get_roof_detect_shape() const;
    Ref<Shape3D> get_wall_detect_shape() const;
};