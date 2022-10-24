#include "GD3Dinterior_area.hpp"

GD3Dinterior_area::GD3Dinterior_area()
{
    uninitialize();
}

GD3Dinterior_area::~GD3Dinterior_area()
{
    uninitialize();
}
#define GETSET_GD3D(FUNC) ClassDB::bind_method(D_METHOD("get_"#FUNC), &GD3Dinterior_area::get_##FUNC);\
                        ClassDB::bind_method(D_METHOD("set_"#FUNC, #FUNC), &GD3Dinterior_area::set_##FUNC)
#define ADDPROP_GD3D(TYPE,PROP) ADD_PROPERTY(PropertyInfo(Variant::##TYPE, #PROP), "set_"#PROP, "get_"#PROP)
void GD3Dinterior_area::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("initialize"), &GD3Dinterior_area::initialize);
    ClassDB::bind_method(D_METHOD("uninitialize"), &GD3Dinterior_area::uninitialize);
    ClassDB::bind_method(D_METHOD("on_enter_area"), &GD3Dinterior_area::on_enter_area);
    ClassDB::bind_method(D_METHOD("on_exit_area"), &GD3Dinterior_area::on_exit_area);

    //ClassDB::bind_method(D_METHOD("get_enter_ignore_bodies"), &GD3Dinterior_area::get_enter_ignore_bodies);
   // ClassDB::bind_method(D_METHOD("get_enter_invisible_array"), &GD3Dinterior_area::get_enter_invisible_array);
    /*
    GETSET_GD3D(on_enter_ignore);
    GETSET_GD3D(on_enter_visible);
    GETSET_GD3D(on_enter_invisible);
    GETSET_GD3D(enter_ignore_node_path);
    GETSET_GD3D(enter_visible_node_path);
    GETSET_GD3D(enter_invisible_node_path);

    ADDPROP_GD3D(BOOL,on_enter_ignore);
    ADDPROP_GD3D(BOOL,on_enter_visible);
    ADDPROP_GD3D(BOOL,on_enter_invisible);
    ADDPROP_GD3D(NODE_PATH,enter_ignore_node_path);
    ADDPROP_GD3D(NODE_PATH,enter_visible_node_path);
    ADDPROP_GD3D(NODE_PATH,enter_invisible_node_path);*/
    ADD_SIGNAL(MethodInfo("entered_signal", PropertyInfo(Variant::RID, "object_rid"), PropertyInfo(Variant::OBJECT, "object"), PropertyInfo(Variant:INT, "ignoremask")));
    ADD_SIGNAL(MethodInfo("exited_signal", PropertyInfo(Variant::RID, "object_rid"), PropertyInfo(Variant::OBJECT, "object"), PropertyInfo(Variant:INT, "ignoremask")));

}
#undef GETSET_GD3D
#undef ADDPROP_GD3D

bool GD3Dinterior_area::initialize()
{
   
    /*uninitialize();

    if (!enter_ignore_node_path.is_empty())
    {
        enter_ignore_node = Object::cast_to<Node>(get_node<Node>(enter_ignore_node_path));
    }
    enter_ignore_bodies.append_array( get_all_sub_physics_bodies(enter_ignore_node));
    
   
    if(!enter_visible_node_path.is_empty())
    {
        enter_visible_node = Object::cast_to<Node3D>(get_node<Node>(enter_visible_node_path));
    }
    if (!enter_invisible_node_path.is_empty())
    {
        enter_invisible_node = Object::cast_to<Node3D>(get_node<Node>(enter_invisible_node_path));
    }

    enter_invisible_array = get_all_sub_visual_obstacles(enter_invisible_node);
    _initialized = true;

    //Initialize all of the visual obstacles to avoid having to add a script in all of them
    if(enter_invisible_array.size()>0)
    {
        for (int64_t i = 0; i < enter_invisible_array.size(); i++)
        {
            GD3Dvisual_obstacle* vis_child = cast_to<GD3Dvisual_obstacle>(enter_invisible_array[i]);
            if (vis_child== nullptr ) continue;

            vis_child->initialize();
        }
    }
    */
    return true;
}

void GD3Dinterior_area::uninitialize()
{
    /*
    enter_ignore_bodies.clear();
    enter_invisible_array.clear();
    enter_ignore_node = nullptr;
    enter_visible_node = nullptr;
    enter_invisible_node = nullptr;
    _initialized = false;*/
}
void GD3Dinterior_area::on_enter_area(uint32_t ignoremask)
{
   /* if (!_initialized) return;
    ignore_layer_array.clear();
    if (on_enter_ignore && enter_ignore_bodies.size()>0)
    {
        for (int64_t i = 0; i < enter_ignore_bodies.size(); i++)
        {
            PhysicsBody3D* n_child = cast_to<PhysicsBody3D>(enter_ignore_bodies[i]);
           
            if (n_child == nullptr ) continue;

            uint32_t msk = n_child->get_collision_layer();
            ignore_layer_array.append(msk);
            n_child->set_collision_layer((msk & ignoremask) ^ msk);
        }
    }

    if(on_enter_invisible && enter_invisible_array.size() > 0)
    {
        for (int64_t i = 0; i < enter_invisible_array.size(); i++)
        {
            GD3Dvisual_obstacle* n_child = cast_to<GD3Dvisual_obstacle>(enter_invisible_array[i]);

            if (n_child == nullptr ) continue;
            n_child->make_invisible();
            n_child->set_locked(true);
        }
    }

    if (on_enter_visible && enter_visible_node != nullptr)
    {
        enter_visible_node->set_visible(true);
    }*/
    emit_signal("entered_signal", get_rid(), this, ignoremask);
}
void GD3Dinterior_area::on_exit_area()
{

   /* if (!_initialized) return;
    int cter = 0;
    if (on_enter_ignore && enter_ignore_bodies.size() > 0)
    {
        for (int64_t i = 0; i < enter_ignore_bodies.size(); i++)
        {
            PhysicsBody3D* n_child = cast_to<PhysicsBody3D>(enter_ignore_bodies[i]);

            if (n_child == nullptr) continue;
            n_child->set_collision_layer(ignore_layer_array[cter]);
            cter++;
        }
    }
    
    if (on_enter_visible && enter_visible_node != nullptr)
    {
        enter_visible_node->set_visible(false);
    }

    if (on_enter_invisible && enter_invisible_array.size() > 0)
    {
        for (int64_t i = 0; i < enter_invisible_array.size(); i++)
        {
            GD3Dvisual_obstacle* n_child = cast_to<GD3Dvisual_obstacle>(enter_invisible_array[i]);

            if (n_child == nullptr) continue;
            n_child->set_locked(false);
            n_child->make_visible();
        }
    }*/

    emit_signal("exited_signal", get_rid(), this, ignoremask);
}
s
//Other functions
/*TypedArray<PhysicsBody3D> GD3Dinterior_area::get_all_sub_physics_bodies(Node* nd)
{
    TypedArray<PhysicsBody3D> pb_array = {};

    if (nd == nullptr) return pb_array;
    
    TypedArray<Node> nd_array = get_all_sub_nodes(nd);

    if (nd_array.size() < 1) return  pb_array;
    for (int64_t i = 0; i < nd_array.size(); i++)
    {
        PhysicsBody3D* pb_child = cast_to<PhysicsBody3D>(nd_array[i]);
        if (pb_child == nullptr) continue;
        
        pb_array.append(pb_child);
    }
    return pb_array;
}
TypedArray<GD3Dvisual_obstacle> GD3Dinterior_area::get_all_sub_visual_obstacles(Node* nd)
{

    TypedArray<GD3Dvisual_obstacle> vis_array = {};

    if (nd == nullptr) return vis_array;

    TypedArray<Node> nd_array = get_all_sub_nodes(nd);

    if (nd_array.size() < 1) return  vis_array;
    for (int64_t i = 0; i < nd_array.size(); i++)
    {
        GD3Dvisual_obstacle* vis_child = cast_to<GD3Dvisual_obstacle>(nd_array[i]);
        if (vis_child == nullptr || vis_child->get_class() != "GD3Dvisual_obstacle") continue;

        vis_array.append(vis_child);
    }
    return vis_array;

}
TypedArray<Node> GD3Dinterior_area::get_all_sub_nodes( Node * nd)
{
    TypedArray<Node> n_arr = {};
    if (nd == nullptr) return n_arr;

    TypedArray<Node> cols = nd->get_children();

    if (cols.size() < 1) return n_arr;
    for (int64_t i = 0; i < cols.size(); i++)
    {
        Node* n_child = cast_to<Node>(cols[i]);
        if (n_child == nullptr) continue;
        n_arr.append(n_child);
        n_arr.append_array(get_all_sub_nodes(n_child));
    }
    return n_arr;
}
//Getters and setters
Node* GD3Dinterior_area::get_enter_ignore_node() { return enter_ignore_node; }
Node3D* GD3Dinterior_area::get_enter_visible_node() { return enter_visible_node; }
Node3D* GD3Dinterior_area::get_enter_invisible_node() { return enter_invisible_node;}

TypedArray<PhysicsBody3D> GD3Dinterior_area::get_enter_ignore_bodies() { return enter_ignore_bodies; }
TypedArray<GD3Dvisual_obstacle> GD3Dinterior_area::get_enter_invisible_array() { return enter_invisible_array; }

bool GD3Dinterior_area::get_on_enter_ignore() const { return on_enter_ignore; }
bool GD3Dinterior_area::get_on_enter_visible() const { return on_enter_visible; }
bool GD3Dinterior_area::get_on_enter_invisible() const { return on_enter_invisible; }

NodePath GD3Dinterior_area::get_enter_ignore_node_path() const { return enter_ignore_node_path;}
NodePath GD3Dinterior_area::get_enter_visible_node_path() const { return enter_visible_node_path;}
NodePath GD3Dinterior_area::get_enter_invisible_node_path() const { return enter_invisible_node_path;}

void GD3Dinterior_area::set_on_enter_ignore(const bool var) { on_enter_ignore = var; }
void GD3Dinterior_area::set_on_enter_visible(const bool var) { on_enter_visible = var; }
void GD3Dinterior_area::set_on_enter_invisible(const bool var) { on_enter_invisible = var; }

void GD3Dinterior_area::set_enter_ignore_node_path(const NodePath& path) { enter_ignore_node_path = path;}
void GD3Dinterior_area::set_enter_visible_node_path(const NodePath& path) { enter_visible_node_path = path;}
void GD3Dinterior_area::set_enter_invisible_node_path(const NodePath& path) { enter_invisible_node_path = path;}*/


