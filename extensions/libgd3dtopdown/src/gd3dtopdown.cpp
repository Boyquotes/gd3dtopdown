#include "GD3Dtopdown.hpp"

GD3Dtopdown::GD3Dtopdown()
{
    initialized = false;
    _uninitialize();
    //Register input actions if they are not registerd beforehand
    InputMap* input_map = InputMap::get_singleton();
    
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

    ClassDB::bind_method(D_METHOD("get_visual_collision_mask"), &GD3Dtopdown::get_visual_collision_mask);
    ClassDB::bind_method(D_METHOD("set_visual_collision_mask", "visual_collision_mask"), &GD3Dtopdown::set_visual_collision_mask);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "visual_collision_mask", PROPERTY_HINT_LAYERS_3D_PHYSICS), "set_visual_collision_mask", "get_visual_collision_mask");


    ClassDB::bind_method(D_METHOD("enter_visual_obstacle_event"), &GD3Dtopdown::enter_visual_obstacle_event);
    ClassDB::bind_method(D_METHOD("exit_visual_obstacle_event"), &GD3Dtopdown::exit_visual_obstacle_event);
    ClassDB::bind_method(D_METHOD("enter_interior_event"), &GD3Dtopdown::enter_interior_event);
    ClassDB::bind_method(D_METHOD("exit_interior_event"), &GD3Dtopdown::exit_interior_event);

   
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
        
        visual_obstacle_collision_area = memnew(Area3D);
        CollisionShape3D* visual_obstacle_col_shp = memnew(CollisionShape3D);
        BoxShape3D* visual_obstacle_box = memnew(BoxShape3D);
        if ( visual_obstacle_collision_area == nullptr || visual_obstacle_col_shp == nullptr 
                || visual_obstacle_box == nullptr)
        {
            _uninitialize();
            ERR_FAIL_V_MSG(false, "Could not initialize visual_obstacle detection areas");
        }
        
        add_child(visual_obstacle_collision_area);
        visual_obstacle_collision_area->set_owner(this);
        visual_obstacle_collision_area->add_child(visual_obstacle_col_shp);
        visual_obstacle_col_shp->set_owner(visual_obstacle_collision_area);
        visual_obstacle_col_shp->set_shape(visual_obstacle_box);

        visual_obstacle_box->set_size(Vector3(1, 1, camera_boon.length()));
        visual_obstacle_col_shp->set_position(Vector3(0, 0, -camera_boon.length()/2.0));

        visual_obstacle_collision_area->set_collision_layer(0);
        visual_obstacle_collision_area->set_collision_mask(visual_collision_mask);
        visual_obstacle_collision_area->set_monitoring(true);
        visual_obstacle_collision_area->set_monitorable(true);
        
        visual_obstacle_collision_area->connect("body_entered", Callable(this, "enter_visual_obstacle_event"));
        visual_obstacle_collision_area->connect("body_exited", Callable(this, "exit_visual_obstacle_event"));

        interiors_collision_area = memnew(Area3D);
        CollisionShape3D* interiors_collision_col_shp = memnew(CollisionShape3D);
        BoxShape3D* interiors_collision_box= memnew(BoxShape3D);

        if (interiors_collision_area == nullptr || interiors_collision_col_shp == nullptr 
                                        || interiors_collision_box == nullptr)
        {
            _uninitialize();
            ERR_FAIL_V_MSG(false, "Could not initialize interiors_collision_area");
        }
        add_child(interiors_collision_area);
        interiors_collision_area->set_owner(this);
        interiors_collision_area->add_child(interiors_collision_col_shp);
        interiors_collision_col_shp->set_owner(interiors_collision_area);
        interiors_collision_col_shp->set_shape(interiors_collision_box);

        interiors_collision_box->set_size(Vector3(1, 1, 1));

        interiors_collision_area->set_collision_layer(0);
        interiors_collision_area->set_collision_mask(visual_collision_mask);
        interiors_collision_area->set_monitoring(true);
        interiors_collision_area->set_monitorable(true);

        interiors_collision_area->connect("area_entered", Callable(this, "enter_interior_event"));
        interiors_collision_area->connect("area_exited", Callable(this, "exit_interior_event"));
        
        visual_obstacle_rayexcludes.clear();
        WARN_PRINT(DEBUG_STR("Initialized GD3D with gravity: " + String::num_real(gravity)));
        initialized = true;
    }
    return initialized;
}
void GD3Dtopdown::_uninitialize()
{
    if (initialized)
    {
       
        if(visual_obstacle_collision_area != nullptr)
        {
            visual_obstacle_collision_area->disconnect("body_entered", Callable(this, "enter_visual_obstacle_event"));
            visual_obstacle_collision_area->disconnect("body_exited", Callable(this, "exit_visual_obstacle_event"));
        }
        if(interiors_collision_area != nullptr)
        {
            interiors_collision_area->disconnect("area_entered", Callable(this, "enter_interior_event"));
            interiors_collision_area->disconnect("area_exited", Callable(this, "exit_interior_event"));
        }
        memfree(visual_obstacle_collision_area);
        memfree(interiors_collision_area);
        
        visual_obstacle_collision_area = nullptr;
        interiors_collision_area = nullptr;
        visual_obstacle_rayexcludes.clear();
        old_aim_node = nullptr;
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
    
#if defined(DEBUG_ENABLED)
    if (Engine::get_singleton()->is_editor_hint()) return;
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
#if defined(DEBUG_ENABLED)
    if (Engine::get_singleton()->is_editor_hint()) return;
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
#if defined(DEBUG_ENABLED) 
if (Engine::get_singleton()->is_editor_hint()) return;
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
    bool is_sprinting = input->is_action_pressed("sprint");
    Vector3 vel = get_velocity();
    bool floored = is_on_floor();
    float speed = walk_speed;

    //Logic with inputs
    if (!floored)
    {
        vel.y -= gravity * delta;
    }

    if (is_sprinting && floored)
    {
        speed = sprint_speed;
    }

    Vector3 direction = (camera->get_transform().basis.xform(Vector3(input_dir.x, 0, input_dir.y))).normalized();
    
    Vector3 lookat_pos = get_position();
    Vector3 front_pos = -get_global_transform().get_basis().get_column(2);
    if (direction.is_zero_approx())
    {
        vel.x = Math::move_toward(vel.x, 0, speed);
        vel.z = Math::move_toward(vel.z, 0, speed);
       
        
        lookat_pos.x +=  front_pos.x;
        lookat_pos.z +=  front_pos.z;
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
                                            4294967295U, visual_obstacle_rayexcludes);

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
    
    camera_follow_position = camera_follow_position.lerp(camera_predict* direction.length_squared() 
                                * front_pos + get_position(), camera_predict_speed * delta);
    
    //Setting the resulting logic to the character and camera nodes
    set_velocity(vel);
    lookat_position = lookat_pos;
    look_at(lookat_position);
    move_and_slide();
    Vector3 cam_pos = camera_follow_position + camera_boon;
    camera->set_position(cam_pos);
    camera->look_at(camera_follow_position);
    visual_obstacle_collision_area->look_at(cam_pos);

}
//Intersects and visual_obstacles separate layers (soon)
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
void GD3Dtopdown::enter_visual_obstacle_event(Variant body)
{
    GD3Dvisual_obstacle* ar = cast_to<GD3Dvisual_obstacle>(body);
    if (ar == nullptr) return;
    ar->emit_visual_disappear();
    RID area_rid = ar->get_rid();
    visual_obstacle_rayexcludes.push_back(area_rid);
}

void GD3Dtopdown::exit_visual_obstacle_event(Variant body)
{
    GD3Dvisual_obstacle* ar = cast_to<GD3Dvisual_obstacle>(body);
    if (ar == nullptr) return;

    ar->emit_visual_appear();
    RID area_rid = ar->get_rid();
    visual_obstacle_rayexcludes.clear();
   
    TypedArray<Node3D> cols = visual_obstacle_collision_area->get_overlapping_bodies();
    
    if (cols.size() < 1) return;
    for (int64_t i = 0; i < cols.size(); i++)
    {
        GD3Dvisual_obstacle* visual_obstacle_a = cast_to<GD3Dvisual_obstacle>(cols[i]);
        if (visual_obstacle_a != nullptr) 
        {
            visual_obstacle_rayexcludes.push_back(visual_obstacle_a->get_rid());
        }
    }
}
void GD3Dtopdown::enter_interior_event(Variant area)
{
    GD3Dinterior_area* ar = cast_to<GD3Dinterior_area>(area);
    if (ar == nullptr) return;
    ar->on_enter_ignore();
}
void GD3Dtopdown::exit_interior_event(Variant area)
{
    GD3Dinterior_area* ar = cast_to<GD3Dinterior_area>(area);
    if (ar == nullptr) return;
    ar->on_enter_ignore();
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

void GD3Dtopdown::set_visual_collision_mask(uint32_t p_mask)
{
    visual_collision_mask = p_mask;
    /*Null checks in setter and getter couse editor to crash 
    if (visual_obstacle_collision_area != nullptr)
    {
        visual_obstacle_collision_area->set_collision_mask(p_mask);
    }*/
}
uint32_t GD3Dtopdown::get_visual_collision_mask() const
{
    uint32_t msk = visual_collision_mask;
   /*
   if (visual_obstacle_collision_area != nullptr)
    {

        msk = visual_obstacle_collision_area->get_collision_mask();
    }*/
    return msk;
}



