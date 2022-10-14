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
    ClassDB::bind_method(D_METHOD("get_aim_node"), &GD3Dtopdown::get_aim_node);
    
    ClassDB::bind_method(D_METHOD("get_camera_node_path"), &GD3Dtopdown::get_camera_node_path);
    ClassDB::bind_method(D_METHOD("set_camera_node_path", "camera_node_path"), &GD3Dtopdown::set_camera_node_path);
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "camera_node_path"), "set_camera_node_path", "get_camera_node_path");

    ClassDB::bind_method(D_METHOD("get_invert_camera_movement"), &GD3Dtopdown::get_invert_camera_movement);
    ClassDB::bind_method(D_METHOD("set_invert_camera_movement", "invert_camera_movement"), &GD3Dtopdown::set_invert_camera_movement);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "invert_camera_movement"), "set_invert_camera_movement", "get_invert_camera_movement");

    ClassDB::bind_method(D_METHOD("get_camera_boon"), &GD3Dtopdown::get_camera_boon);
    ClassDB::bind_method(D_METHOD("set_camera_boon", "camera_boon"), &GD3Dtopdown::set_camera_boon);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "camera_boon"), "set_camera_boon", "get_camera_boon");

    ClassDB::bind_method(D_METHOD("get_camera_predict"), &GD3Dtopdown::get_camera_predict);
    ClassDB::bind_method(D_METHOD("set_camera_predict", "camera_predict"), &GD3Dtopdown::set_camera_predict);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "camera_predict"), "set_camera_predict", "get_camera_predict");

    ClassDB::bind_method(D_METHOD("get_camera_predict_speed"), &GD3Dtopdown::get_camera_predict_speed);
    ClassDB::bind_method(D_METHOD("set_camera_predict_speed", "camera_predict_speed"), &GD3Dtopdown::set_camera_predict_speed);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "camera_predict_speed"), "set_camera_predict_speed", "get_camera_predict_speed");

    ClassDB::bind_method(D_METHOD("set_roof_detect_shape", "roof_detect_shape"), &GD3Dtopdown::set_roof_detect_shape);
    ClassDB::bind_method(D_METHOD("get_roof_detect_shape"), &GD3Dtopdown::get_roof_detect_shape);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "roof_detect_shape", PROPERTY_HINT_RESOURCE_TYPE, "Shape3D"), "set_roof_detect_shape", "get_roof_detect_shape");

    ClassDB::bind_method(D_METHOD("set_wall_detect_shape", "wall_detect_shape"), &GD3Dtopdown::set_wall_detect_shape);
    ClassDB::bind_method(D_METHOD("get_wall_detect_shape"), &GD3Dtopdown::get_wall_detect_shape);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "wall_detect_shape", PROPERTY_HINT_RESOURCE_TYPE, "Shape3D"), "set_wall_detect_shape", "get_wall_detect_shape");

    ClassDB::bind_method(D_METHOD("enter_wall_event"), &GD3Dtopdown::enter_wall_event);
    ClassDB::bind_method(D_METHOD("enter_roof_event"), &GD3Dtopdown::enter_roof_event);
    ClassDB::bind_method(D_METHOD("exit_wall_event"), &GD3Dtopdown::exit_wall_event);
    ClassDB::bind_method(D_METHOD("exit_roof_event"), &GD3Dtopdown::exit_roof_event);

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
        all_rayexcludes.clear();
        wall_rayexcludes.clear();
        roof_rayexcludes.clear();

        //rayexcludes.clear();
        //rayexcludes.push_back(get_rid());
        //rayexcludes.push_back(camera->get_camera_rid());
        initialized = true;
        WARN_PRINT(DEBUG_STR("Initialized GD3D with gravity: " + String::num_real(gravity)));
        
        
        roof_collision_area = memnew(Area3D);
        wall_collision_area = memnew(Area3D);
        if (roof_collision_area == nullptr || wall_collision_area == nullptr) 
        {
            _uninitialize();
            ERR_FAIL_V_MSG(false, "Could not initialize roof or wall detection areas");
        }
        add_child(roof_collision_area);
        roof_collision_area->set_owner(this);
        add_child(wall_collision_area);
        wall_collision_area->set_owner(this);

        CollisionShape3D* roof_col_shp = memnew(CollisionShape3D);
        CollisionShape3D* wall_col_shp = memnew(CollisionShape3D);

        if (roof_col_shp == nullptr || wall_col_shp == nullptr)
        {
            _uninitialize();
            ERR_FAIL_V_MSG(false, "Could not initialize roof or wall detection nodes");
        }


        //All this is better done in editor
        roof_collision_area->add_child(roof_col_shp);
        roof_collision_area->set_owner(roof_collision_area);

        wall_collision_area->add_child(wall_col_shp);
        wall_collision_area->set_owner(wall_collision_area);

        if (wall_detect_shape.is_null() || wall_detect_shape.is_null())
        {
            _uninitialize();
            ERR_FAIL_V_MSG(false, "Could not initialize roof or wall detection shapes");
        }
        //wall_col_shp->set_shape(wall_detect_shape);
       // roof_col_shp->set_shape(roof_detect_shape);

        BoxShape3D* roof_box = memnew(BoxShape3D);
        BoxShape3D* wall_box = memnew(BoxShape3D);
        roof_box->set_size(Vector3(1, 20, 1));

        wall_box->set_size(Vector3(1, 20, 1));
        wall_col_shp->set_shape(wall_detect_shape);
        roof_col_shp->set_shape(roof_box);
        roof_collision_area->set_position(Vector3(0, 5, 0));
        wall_collision_area->set_monitoring(true);
        roof_collision_area->set_monitoring(true);

        wall_collision_area->connect("body_entered", Callable(this, "enter_wall_event"));
        roof_collision_area->connect("body_entered", Callable(this,"enter_roof_event"));
        wall_collision_area->connect("body_exited", Callable(this,"exit_wall_event"));
        roof_collision_area->connect("body_exited", Callable(this,"exit_roof_event"));
    }
    return initialized;
}
void GD3Dtopdown::_uninitialize()
{
    if (initialized)
    {
        memfree(wall_collision_area);
        memfree(roof_collision_area);
        wall_collision_area = nullptr;
        roof_collision_area = nullptr;

        roof_rayexcludes.clear();
        wall_rayexcludes.clear();
        all_rayexcludes.clear();
        old_aim_node = nullptr;
        old_roof_node = nullptr;
        old_intersect_node = nullptr;
        p_settings = nullptr;
        input = nullptr;
        camera = nullptr;
        ph_server = nullptr;
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
Calling Engine::get_singleton()->is_editor_hint() does not solve the issue as the functions are not ticking inside the Run
*/
void GD3Dtopdown::_ready()
{
    if (Engine::get_singleton()->is_editor_hint()) return;
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
    if (Engine::get_singleton()->is_editor_hint()) return;
#if defined(DEBUG_ENABLED)
    WARN_PRINT_ONCE("Input function called");
}
void GD3Dtopdown::_input_handle(const Ref<InputEvent>& p_event)
{
    WARN_PRINT_ONCE("Input handle function called");
#endif

    if (!initialized) return;
    Ref<InputEventMouseMotion> m = p_event;
    if (m.is_valid() && !is_aiming)
    {
        Vector2 motion = m->get_relative();
        float xmov = Math::deg_to_rad(motion.x);
        if (invert_camera_movement) xmov *= -1;
        camera_boon.rotate( Vector3(0,1,0), xmov * mouse_sensitivity);
        camera->set_position(camera_follow_position + camera_boon);
        camera->look_at(camera_follow_position);
    }
    return;
}

void GD3Dtopdown::_physics_process(double delta)
{
    if (Engine::get_singleton()->is_editor_hint()) return;
#if defined(DEBUG_ENABLED) 
    WARN_PRINT_ONCE("Physics process called");
}
void GD3Dtopdown::_physics_process_handle(double delta)
{
    WARN_PRINT_ONCE("Physics handle process called");
#endif

    if (!initialized) return;
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
    
    Vector3 lookat_pos = get_position();
    if (direction.is_zero_approx())
    {
        vel.x = Math::move_toward(vel.x, 0, speed);
        vel.z = Math::move_toward(vel.z, 0, speed);
       
        Vector3 front_pos = get_global_transform().get_basis().get_column(2);
        lookat_pos.x -=  front_pos.x;
        lookat_pos.z -=  front_pos.z;
    }
    else
    {
        vel.x = direction.x * speed;
        vel.z = direction.z * speed;
        lookat_pos.x += direction.x;
        lookat_pos.z += direction.z;
    }

    if (is_aiming)
    {

        Vector2 mouse_pos = get_viewport()->get_mouse_position();

        Ref<PhysicsRayQueryParameters3D> ray = PhysicsRayQueryParameters3D::create(
                                            camera->project_ray_origin(mouse_pos), 
                                            camera->project_ray_normal(mouse_pos) * 1000,
                                            4294967295U, all_rayexcludes);

        Dictionary ray_dict = ph_server->space_get_direct_state(
                                            w3d->get_space())->intersect_ray(ray);
        if(ray_dict != Dictionary())
        {
            Vector3 pos = ray_dict["position"];
            lookat_pos.x = pos.x;
            lookat_pos.z = pos.z;
          
            Node3D* nd = cast_to<Node3D>(ray_dict["collider"]);
            handle_aim_node(nd);
        }
        else
        {
            handle_aim_node(nullptr);
        }
    }
    else
    {
        if(old_aim_node != nullptr)
        {
            //Deselect function here
            old_aim_node = nullptr;
        }
    }
    
    camera_follow_position = camera_follow_position.lerp(camera_predict * direction + get_position(), camera_predict_speed * delta);
    
    //Setting the resulting logic to the character and camera nodes
    set_velocity(vel);
    move_and_slide();
    lookat_position = lookat_pos;
    look_at(lookat_position);
    
    camera->set_position(camera_follow_position + camera_boon);
    camera->look_at(camera_follow_position);
}
//Intersects and walls separate layers (soon)
//Other functions

void GD3Dtopdown::handle_aim_node(Node3D* nd)
{
    
    if (old_aim_node == nd) return;
    if (old_aim_node != nullptr)
    {
        
        //DeHighlight function old_aim_node_->
    }
    old_aim_node = nd;

    if( nd != nullptr)
    {
        //Highlight_function
    }
}
void GD3Dtopdown::enter_wall_event(Variant area)
{
    PhysicsBody3D* ar = cast_to<PhysicsBody3D>(area);
    if (ar == nullptr) return;
    RID area_rid = ar->get_rid();
    wall_rayexcludes.push_back(area_rid);
    all_rayexcludes.push_back(area_rid);

}
void GD3Dtopdown::enter_roof_event(Variant area)
{
    PhysicsBody3D* ar = cast_to<PhysicsBody3D>(area);
    if (ar == nullptr) return;
    RID area_rid = ar->get_rid();
    roof_rayexcludes.push_back(area_rid);
    all_rayexcludes.push_back(area_rid);
}
void GD3Dtopdown::exit_wall_event( Variant area)
{
    PhysicsBody3D* ar = cast_to<PhysicsBody3D>(area);
    if (ar == nullptr) return;
    RID area_rid = ar->get_rid();
    wall_rayexcludes.clear();
    all_rayexcludes.clear();
    all_rayexcludes = roof_rayexcludes;
    TypedArray<Area3D> cols = wall_collision_area->get_overlapping_areas();
    
    for (int64_t i = 0; i < cols.size(); i++)
    {
        Area3D* wall_a = cast_to<Area3D>(cols[i]);
        if (wall_a != nullptr) 
        {
            wall_rayexcludes.push_back(wall_a->get_rid());
            all_rayexcludes.push_back(wall_a->get_rid());
        }
    }
}
void GD3Dtopdown::exit_roof_event(Variant area)
{
    PhysicsBody3D* ar = cast_to<PhysicsBody3D>(area);
    if (ar == nullptr) return;
    RID area_rid = ar->get_rid();
    roof_rayexcludes.clear();
    all_rayexcludes.clear();
    all_rayexcludes = wall_rayexcludes;

    TypedArray<Area3D> cols = roof_collision_area->get_overlapping_areas();

    for (int64_t i = 0; i < cols.size(); i++)
    {
        Area3D* wall_a = cast_to<Area3D>(cols[i]);
        if (wall_a != nullptr)
        {
            roof_rayexcludes.push_back(wall_a->get_rid());
            all_rayexcludes.push_back(wall_a->get_rid());
        }
    }
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
void GD3Dtopdown::set_invert_camera_movement(const bool inv)
{
    invert_camera_movement = inv;
}
void GD3Dtopdown::set_wall_detect_shape(Ref<Shape3D> shape)
{
    wall_detect_shape = shape;
}
void GD3Dtopdown::set_roof_detect_shape(Ref<Shape3D> shape)
{
    roof_detect_shape = shape;
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
bool GD3Dtopdown::get_invert_camera_movement() const 
{
    return invert_camera_movement;
};
Node3D* GD3Dtopdown::get_aim_node()const
{
    return old_aim_node;
}
Ref<Shape3D> GD3Dtopdown::get_roof_detect_shape() const
{
    return roof_detect_shape;
}
Ref<Shape3D> GD3Dtopdown::get_wall_detect_shape() const
{
    return wall_detect_shape;
}


