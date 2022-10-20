#include "GD3Dtd_character.hpp"

GD3Dtd_character::GD3Dtd_character()
{
    initialized = false;
    _uninitialize();
    //Register input actions if they are not registerd beforehand
    InputMap* input_map = InputMap::get_singleton();

    if (!input_map->has_action("sprint"))       input_map->add_action("sprint");
    if (!input_map->has_action("move_left"))    input_map->add_action("move_left");
    if (!input_map->has_action("move_right"))   input_map->add_action("move_right");
    if (!input_map->has_action("move_forward")) input_map->add_action("move_forward");
    if (!input_map->has_action("move_back"))    input_map->add_action("move_back");
    if (!input_map->has_action("aim"))    input_map->add_action("aim");
}
GD3Dtd_character::~GD3Dtd_character()
{
    _uninitialize();
}

#define GETSET_GD3D(FUNC) ClassDB::bind_method(D_METHOD("get_"#FUNC), &GD3Dtd_character::get_##FUNC);\
                        ClassDB::bind_method(D_METHOD("set_"#FUNC, #FUNC), &GD3Dtd_character::set_##FUNC)
#define ADDPROP_GD3D(PROP,TYPE) ADD_PROPERTY(PropertyInfo(Variant::##TYPE, #PROP), "set_"#PROP, "get_"#PROP)
void GD3Dtd_character::_bind_methods()
{

#if  defined(DEBUG_ENABLED) 
    ClassDB::bind_method(D_METHOD("_ready_handle"), &GD3Dtd_character::_ready_handle);
    ClassDB::bind_method(D_METHOD("_input_handle"), &GD3Dtd_character::_input_handle);
    ClassDB::bind_method(D_METHOD("_physics_process_handle"), &GD3Dtd_character::_physics_process_handle);
#endif

    GETSET_GD3D(mouse_sensitivity);
    GETSET_GD3D(walk_speed);
    GETSET_GD3D(sprint_speed);
    GETSET_GD3D(camera_node_path);
    GETSET_GD3D(invert_camera_movement);
    GETSET_GD3D(camera_boon);
    GETSET_GD3D(camera_predict);
    GETSET_GD3D(camera_predict_speed);
    GETSET_GD3D(interiors_collision_mask);
    GETSET_GD3D(visual_collision_mask);
    GETSET_GD3D(aim_collision_mask);

    ClassDB::bind_method(D_METHOD("get_lookat_position"), &GD3Dtd_character::get_lookat_position);
    ClassDB::bind_method(D_METHOD("get_aim_node"), &GD3Dtd_character::get_aim_node);

    ClassDB::bind_method(D_METHOD("enter_visual_event"), &GD3Dtd_character::enter_visual_event);
    ClassDB::bind_method(D_METHOD("exit_visual_event"), &GD3Dtd_character::exit_visual_event);
    ClassDB::bind_method(D_METHOD("enter_interior_event"), &GD3Dtd_character::enter_interior_event);
    ClassDB::bind_method(D_METHOD("exit_interior_event"), &GD3Dtd_character::exit_interior_event);

    ADDPROP_GD3D(mouse_sensitivity, FLOAT);
    ADDPROP_GD3D(walk_speed, FLOAT);
    ADDPROP_GD3D(sprint_speed, FLOAT);
    ADDPROP_GD3D(camera_node_path, NODE_PATH);
    ADDPROP_GD3D(invert_camera_movement, BOOL);
    ADDPROP_GD3D(camera_boon, VECTOR3);
    ADDPROP_GD3D(camera_predict, FLOAT);
    ADDPROP_GD3D(camera_predict_speed, FLOAT);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "interiors_collision_mask", PROPERTY_HINT_LAYERS_3D_PHYSICS), "set_interiors_collision_mask", "get_interiors_collision_mask");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "visual_collision_mask", PROPERTY_HINT_LAYERS_3D_PHYSICS), "set_visual_collision_mask", "get_visual_collision_mask");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "aim_collision_mask", PROPERTY_HINT_LAYERS_3D_PHYSICS), "set_aim_collision_mask", "get_aim_collision_mask");
}
#undef GETSET_GD3D
#undef ADDPROP_GD3D
//Initializer functions, get pointers to Input and project settings. Set gravity value
#define CHECKNULL_ERRGD3D(PTR) if( PTR == nullptr ){ _uninitialize() ; ERR_FAIL_V_MSG(false, "Could not obtain "#PTR" pointer, didnt initialize");}
bool GD3Dtd_character::_initialize()
{
    if (!initialized)
    {
        //General pointers
        p_settings = ProjectSettings::get_singleton();
        input = Input::get_singleton();
        ph_server = PhysicsServer3D::get_singleton();
        camera = Object::cast_to<Camera3D>(get_node<Node>(camera_node_path));

        //Area3D pointers
        visual_collision_area = memnew(Area3D);
        CollisionShape3D* visual_col_shp = memnew(CollisionShape3D);
        BoxShape3D* visual_box = memnew(BoxShape3D);

        interiors_collision_area = memnew(Area3D);
        CollisionShape3D* interiors_collision_col_shp = memnew(CollisionShape3D);
        BoxShape3D* interiors_collision_box = memnew(BoxShape3D);

        CHECKNULL_ERRGD3D(p_settings);
        CHECKNULL_ERRGD3D(input);
        CHECKNULL_ERRGD3D(ph_server);
        CHECKNULL_ERRGD3D(camera);

        CHECKNULL_ERRGD3D(visual_collision_area);
        CHECKNULL_ERRGD3D(visual_col_shp);
        CHECKNULL_ERRGD3D(visual_box);

        CHECKNULL_ERRGD3D(interiors_collision_area);
        CHECKNULL_ERRGD3D(interiors_collision_col_shp);
        CHECKNULL_ERRGD3D(interiors_collision_box);

        if((visual_collision_mask & aim_collision_mask) != 0)
        {
            WARN_PRINT("Visual_collision_mask and aim_collision_mask share layers this might lead to unexpected behavior");
        }
        if ((interiors_collision_mask & aim_collision_mask) != 0)
        {
            WARN_PRINT("interiors_collision_mask and aim_collision_mask share layers this might lead to unexpected behavior");
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
        //Visual obstacle Collision area initialization
        add_child(visual_collision_area);
        visual_collision_area->set_owner(this);
        visual_collision_area->add_child(visual_col_shp);
        visual_col_shp->set_owner(visual_collision_area);
        visual_col_shp->set_shape(visual_box);

        visual_box->set_size(Vector3(1, 1, camera_boon.length()));
        visual_col_shp->set_position(Vector3(0, 0, -camera_boon.length() / 2.0));

        visual_collision_area->set_collision_layer(0);
        visual_collision_area->set_collision_mask(visual_collision_mask);
        visual_collision_area->set_monitoring(true);
        visual_collision_area->set_monitorable(true);

        visual_collision_area->connect("body_entered", Callable(this, "enter_visual_event"));
        visual_collision_area->connect("body_exited", Callable(this, "exit_visual_event"));

        //Interior Collision area initialization
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

        visual_rayexcludes.clear();
        WARN_PRINT(DEBUG_STR("Initialized GD3D with gravity: " + String::num_real(gravity)));
        initialized = true;
    }
    return initialized;
}
#undef CHECKNULL_ERRGD3D
void GD3Dtd_character::_uninitialize()
{
    if (initialized)
    {

        if (visual_collision_area != nullptr)
        {
            visual_collision_area->disconnect("body_entered", Callable(this, "enter_visual_event"));
            visual_collision_area->disconnect("body_exited", Callable(this, "exit_visual_event"));
        }
        if (interiors_collision_area != nullptr)
        {
            interiors_collision_area->disconnect("area_entered", Callable(this, "enter_interior_event"));
            interiors_collision_area->disconnect("area_exited", Callable(this, "exit_interior_event"));
        }
        memfree(visual_collision_area);
        memfree(interiors_collision_area);

        visual_collision_area = nullptr;
        interiors_collision_area = nullptr;
        visual_rayexcludes.clear();
        old_aim_node = nullptr;
        p_settings = nullptr;
        input = nullptr;
        camera = nullptr;
        ph_server = nullptr;
        initialized = false;
    }
    return;
}
bool GD3Dtd_character::_is_initialized() const
{
    return initialized;
}

/*Overriden functions _ready, _inputand _physics_process seem to be called
on object instantiation in the editor but are not called during the game (Tested in debug and release,
will keep the preprocessor directives in case im missing something but if you intend to use it remove them as godot wont find the functions when exported if not)
this makes getting pointers to singletos or executing code impossible from them
in order to account for this, The functions are defined as {func}_handle and called from gdscript
see GD3Dtd_character_project/GD3Dtd_character_godot/scripts/GD3Dtd_character.gd
Calling Engine::get_singleton()->is_editor_hint() does not solve the issue as the functions are not ticking inside the Run
*/
void GD3Dtd_character::_ready()
{

#if defined(DEBUG_ENABLED)
    if (Engine::get_singleton()->is_editor_hint()) return;
}
void GD3Dtd_character::_ready_handle()
{
#endif
    _initialize();
}

void GD3Dtd_character::_input(const Ref<InputEvent>& p_event)
{
#if defined(DEBUG_ENABLED)
    if (Engine::get_singleton()->is_editor_hint()) return;
}
void GD3Dtd_character::_input_handle(const Ref<InputEvent>& p_event)
{
#endif

    if (!initialized) return;
    Ref<InputEventMouseMotion> m = p_event;
    if (m.is_valid() && !is_aiming)
    {
        Vector2 motion = m->get_relative();
        float xmov = Math::deg_to_rad(motion.x);
        if (invert_camera_movement) xmov *= -1;
        camera_boon.rotate(Vector3(0, 1, 0), xmov * mouse_sensitivity);
        camera->set_position(camera_follow_position + camera_boon);
        camera->look_at(camera_follow_position);
    }
    return;
}

void GD3Dtd_character::_physics_process(double delta)
{
#if defined(DEBUG_ENABLED) 
    if (Engine::get_singleton()->is_editor_hint()) return;
    WARN_PRINT_ONCE("Physics process called");
}
void GD3Dtd_character::_physics_process_handle(double delta)
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


        lookat_pos.x += front_pos.x;
        lookat_pos.z += front_pos.z;
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
            aim_collision_mask, visual_rayexcludes);

        Dictionary ray_dict = ph_server->space_get_direct_state(
            w3d->get_space())->intersect_ray(ray);
        if (ray_dict != Dictionary())
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
        if (old_aim_node != nullptr)
        {
            //Deselect function here
            old_aim_node = nullptr;
        }
    }

    camera_follow_position = camera_follow_position.lerp(camera_predict * direction.length_squared()
        * front_pos + get_position(), camera_predict_speed * delta);

    //Setting the resulting logic to the character and camera nodes
    set_velocity(vel);
    lookat_position = lookat_pos;
    look_at(lookat_position);
    move_and_slide();
    Vector3 cam_pos = camera_follow_position + camera_boon;
    camera->set_position(cam_pos);
    camera->look_at(camera_follow_position);
    visual_collision_area->look_at(cam_pos);

}
//Intersects and visual_obstacles separate layers (soon)
//Other functions

void GD3Dtd_character::handle_aim_node(Node3D* nd)
{
    if (old_aim_node == nd) return;
    if (old_aim_node != nullptr)
    {
        //DeHighlight function old_aim_node_->
    }
    old_aim_node = nd;

    if (nd != nullptr)
    {
        //Highlight_function
    }
}
void GD3Dtd_character::enter_visual_event(Variant body)
{
    GD3Dvisual_obstacle* ar = cast_to<GD3Dvisual_obstacle>(body);
    if (ar == nullptr) return;
    ar->on_enter_obstacle();
    RID obstacle = ar->get_rid();
    visual_rayexcludes.push_back(obstacle);
}

void GD3Dtd_character::exit_visual_event(Variant body)
{
    GD3Dvisual_obstacle* ar = cast_to<GD3Dvisual_obstacle>(body);
    if (ar == nullptr) return;

    ar->on_exit_obstacle();
    RID obstacle = ar->get_rid();
    visual_rayexcludes.clear();

    TypedArray<Node3D> cols = visual_collision_area->get_overlapping_bodies();

    if (cols.size() < 1) return;
    for (int64_t i = 0; i < cols.size(); i++)
    {
        GD3Dvisual_obstacle* visual_a = cast_to<GD3Dvisual_obstacle>(cols[i]);
        if (visual_a == nullptr) continue;
        visual_rayexcludes.push_back(visual_a->get_rid());
    }
}
void GD3Dtd_character::enter_interior_event(Variant area)
{
    GD3Dinterior_area* in_area = cast_to<GD3Dinterior_area>(area);
    if (in_area == nullptr) return;
    in_area->on_enter(aim_collision_mask);
}
void GD3Dtd_character::exit_interior_event(Variant area)
{
    GD3Dinterior_area* in_area = cast_to<GD3Dinterior_area>(area);
    if (in_area == nullptr) return;
    in_area->on_exit(aim_collision_mask);
}

//Setters and getters
#define GETTERSETTER_GD3D(VAR,TYPE) void GD3Dtd_character::set_##VAR##(const TYPE##& set) { VAR = set;}\
                                            TYPE GD3Dtd_character::get_##VAR##() const {return VAR ;}
GETTERSETTER_GD3D(mouse_sensitivity, float);
GETTERSETTER_GD3D(walk_speed, float);
GETTERSETTER_GD3D(sprint_speed, float);
GETTERSETTER_GD3D(camera_node_path, NodePath);
GETTERSETTER_GD3D(invert_camera_movement, bool);
GETTERSETTER_GD3D(camera_boon, Vector3);
GETTERSETTER_GD3D(camera_predict, float);
GETTERSETTER_GD3D(camera_predict_speed, float);
GETTERSETTER_GD3D(aim_collision_mask, uint32_t);
GETTERSETTER_GD3D(interiors_collision_mask, uint32_t);

#undef GETTERSETTER_GD3D
Vector3 GD3Dtd_character::get_lookat_position() const
{
    return lookat_position;
}
Node3D* GD3Dtd_character::get_aim_node()const
{
    return old_aim_node;
}

void GD3Dtd_character::set_visual_collision_mask(uint32_t p_mask)
{
    visual_collision_mask = p_mask;
    /*Null checks in setter and getter couse editor to crash
    if (visual_collision_area == nullptr) return;

    visual_collision_area->set_collision_mask(p_mask);
    return;*/
}
uint32_t GD3Dtd_character::get_visual_collision_mask() const
{
    uint32_t msk = visual_collision_mask;
    /*
    if (visual_collision_area != nullptr) return msk;

    msk = visual_collision_area->get_collision_mask();
     }*/
    return msk;
}
