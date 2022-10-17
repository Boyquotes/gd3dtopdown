#include "GD3Dinterior_area.hpp"

#define GETSET_GD3D(FUNC) ClassDB::bind_method(D_METHOD("get_"#FUNC), &GD3Dinterior_area::get_##FUNC##);\
                        ClassDB::bind_method(D_METHOD("set_"#FUNC, #FUNC), &GD3Dinterior_area::set_##FUNC##)
#define ADDPROP_GD3D(TYPE,PROP) ADD_PROPERTY(PropertyInfo(Variant::##TYPE##, #PROP), "set_"#PROP, "get_"#PROP)

#define INITFROMNODEPATH_GD3D(NODENAME) NODENAME = Object::cast_to<Node>(get_node<Node>(##NODENAME##_path));\
                                        if (##NODENAME##== nullptr) ERR_FAIL_V_MSG(false, "Could not obtain reference node "#NODENAME);

GD3Dinterior_area::GD3Dinterior_area()
{
}

GD3Dinterior_area::~GD3Dinterior_area()
{
    uninitialize();
}
void GD3Dinterior_area::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("on_enter_ignore"), &GD3Dinterior_area::on_enter_ignore);
    ClassDB::bind_method(D_METHOD("initialize"), &GD3Dinterior_area::initialize);
    ClassDB::bind_method(D_METHOD("uninitialize"), &GD3Dinterior_area::uninitialize);

    GETSET_GD3D(enter_ignore_node_path);
    GETSET_GD3D(exit_ignore_node_path);
    GETSET_GD3D(enter_unignore_node_path);
    GETSET_GD3D(exit_unignore_node_path);
    GETSET_GD3D(enter_visible_node_path);
    GETSET_GD3D(exit_visible_node_path);
    GETSET_GD3D(enter_invisible_node_path);
    GETSET_GD3D(exit_invisible_node_path);

    ADDPROP_GD3D(NODE_PATH,enter_ignore_node_path);
    ADDPROP_GD3D(NODE_PATH,exit_ignore_node_path);
    ADDPROP_GD3D(NODE_PATH,enter_unignore_node_path);
    ADDPROP_GD3D(NODE_PATH,exit_unignore_node_path);
    ADDPROP_GD3D(NODE_PATH,enter_visible_node_path);
    ADDPROP_GD3D(NODE_PATH,exit_visible_node_path);
    ADDPROP_GD3D(NODE_PATH,enter_invisible_node_path);
    ADDPROP_GD3D(NODE_PATH,exit_invisible_node_path);

}
bool GD3Dinterior_area::initialize()
{
    INITFROMNODEPATH_GD3D(enter_ignore_node);
    INITFROMNODEPATH_GD3D(exit_ignore_node);
    INITFROMNODEPATH_GD3D(enter_unignore_node);
    INITFROMNODEPATH_GD3D(exit_unignore_node);
    INITFROMNODEPATH_GD3D(enter_visible_node);
    INITFROMNODEPATH_GD3D(exit_visible_node);
    INITFROMNODEPATH_GD3D(enter_invisible_node);
    INITFROMNODEPATH_GD3D(exit_invisible_node);

    enter_ignore_subnodes = get_all_sub_physics_bodies(enter_ignore_node);
    exit_ignore_subnodes = get_all_sub_physics_bodies(exit_ignore_node);
    enter_unignore_subnodes = get_all_sub_physics_bodies(enter_unignore_node);
    exit_unignore_subnodes = get_all_sub_physics_bodies(exit_unignore_node);
    enter_visible_subnodes = get_all_sub_nodes(enter_visible_node);
    exit_visible_subnodes = get_all_sub_nodes(exit_visible_node);
    enter_invisible_subnodes = get_all_sub_nodes(enter_invisible_node);
    exit_invisible_subnodes = get_all_sub_nodes(exit_invisible_node);

    return true;
}
void GD3Dinterior_area::uninitialize()
{
    enter_ignore_node = nullptr;
    exit_ignore_node = nullptr;
    enter_unignore_node = nullptr;
    exit_unignore_node = nullptr;
    enter_visible_node = nullptr;
    exit_visible_node = nullptr;
    enter_invisible_node = nullptr;
    exit_invisible_node = nullptr;
}
void GD3Dinterior_area::on_enter_ignore()
{
    if (enter_ignore_subnodes.size() < 1) return;

    for (int64_t i = 0; i < enter_ignore_subnodes.size(); i++)
    {
        WARN_PRINT(cast_to<PhysicsBody3D>(enter_ignore_subnodes[i])->get_name());
    }
}
TypedArray<PhysicsBody3D> GD3Dinterior_area::get_all_sub_physics_bodies(Node* nd)
{
    TypedArray<PhysicsBody3D> pb_array = {};
    TypedArray<Node> nd_array = get_all_sub_nodes(nd);
    if (nd_array.size() < 1) return  pb_array;
    for (int64_t i = 0; i < nd_array.size(); i++)
    {
        PhysicsBody3D* pb_child = cast_to<PhysicsBody3D>(nd_array[i]);
        if (pb_child != nullptr)
        {
            pb_array.append(pb_child);
        }
    }
    return pb_array;
    
}
TypedArray<Node> GD3Dinterior_area::get_all_sub_nodes( Node * nd)
{
    TypedArray<Node> cols = nd->get_children();
    TypedArray<Node> n_arr = {};
    if (cols.size() < 1) return n_arr;
    for (int64_t i = 0; i < cols.size(); i++)
    {
        Node* n_child = cast_to<Node>(cols[i]);
        if (n_child != nullptr)
        {
            n_arr.append(n_child);
            n_arr.append_array(get_all_sub_nodes(n_child));
        }
    }
    return n_arr;
    
}
NodePath GD3Dinterior_area::get_enter_ignore_node_path() const { return enter_ignore_node_path;}
NodePath GD3Dinterior_area::get_exit_ignore_node_path() const { return exit_ignore_node_path;}
NodePath GD3Dinterior_area::get_enter_unignore_node_path() const { return enter_unignore_node_path;}
NodePath GD3Dinterior_area::get_exit_unignore_node_path() const { return exit_unignore_node_path;}
NodePath GD3Dinterior_area::get_enter_visible_node_path() const { return enter_visible_node_path;}
NodePath GD3Dinterior_area::get_exit_visible_node_path() const { return exit_visible_node_path;}
NodePath GD3Dinterior_area::get_enter_invisible_node_path() const { return enter_invisible_node_path;}
NodePath GD3Dinterior_area::get_exit_invisible_node_path() const { return exit_invisible_node_path; }

void GD3Dinterior_area::set_enter_ignore_node_path(const NodePath& path) { enter_ignore_node_path = path;}
void GD3Dinterior_area::set_exit_ignore_node_path(const NodePath& path) { exit_ignore_node_path = path;}
void GD3Dinterior_area::set_enter_unignore_node_path(const NodePath& path) { enter_unignore_node_path = path;}
void GD3Dinterior_area::set_exit_unignore_node_path(const NodePath& path) { exit_unignore_node_path = path;}
void GD3Dinterior_area::set_enter_visible_node_path(const NodePath& path) { enter_visible_node_path = path;}
void GD3Dinterior_area::set_exit_visible_node_path(const NodePath& path) { exit_visible_node_path = path;}
void GD3Dinterior_area::set_enter_invisible_node_path(const NodePath& path) { enter_invisible_node_path = path;}
void GD3Dinterior_area::set_exit_invisible_node_path(const NodePath& path) { exit_invisible_node_path = path;}
