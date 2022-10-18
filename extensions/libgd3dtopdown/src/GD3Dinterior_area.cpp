#include "GD3Dinterior_area.hpp"

GD3Dinterior_area::GD3Dinterior_area()
{
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
    ClassDB::bind_method(D_METHOD("on_enter"), &GD3Dinterior_area::on_enter);
    ClassDB::bind_method(D_METHOD("on_exit"), &GD3Dinterior_area::on_exit);

    ClassDB::bind_method(D_METHOD("get_enter_ignore_subnodes"), &GD3Dinterior_area::get_enter_ignore_subnodes);
    ClassDB::bind_method(D_METHOD("get_enter_unignore_subnodes"), &GD3Dinterior_area::get_enter_unignore_subnodes);
    ClassDB::bind_method(D_METHOD("get_enter_ignore_subnodes_as_rid"), &GD3Dinterior_area::get_enter_ignore_subnodes);
    ClassDB::bind_method(D_METHOD("get_enter_unignore_subnodes_as_rid"), &GD3Dinterior_area::get_enter_unignore_subnodes);
    ClassDB::bind_method(D_METHOD("get_enter_visible_subnodes"), &GD3Dinterior_area::get_enter_visible_subnodes);
    ClassDB::bind_method(D_METHOD("get_enter_invisible_subnodes"), &GD3Dinterior_area::get_enter_invisible_subnodes);


    GETSET_GD3D(enter_ignore_node_path);
    GETSET_GD3D(enter_unignore_node_path);
    GETSET_GD3D(enter_visible_node_path);
    GETSET_GD3D(enter_invisible_node_path);
    ADDPROP_GD3D(NODE_PATH,enter_ignore_node_path);
    ADDPROP_GD3D(NODE_PATH,enter_unignore_node_path);
    ADDPROP_GD3D(NODE_PATH,enter_visible_node_path);
    ADDPROP_GD3D(NODE_PATH,enter_invisible_node_path);

    ADD_SIGNAL(MethodInfo("entered_signal", PropertyInfo(Variant::RID, "object_rid"), PropertyInfo(Variant::OBJECT, "object")));
    ADD_SIGNAL(MethodInfo("exited_signal", PropertyInfo(Variant::RID, "object_rid"), PropertyInfo(Variant::OBJECT, "object")));

    ADD_SIGNAL(MethodInfo("entered_with_subnodes", PropertyInfo(Variant::RID, "object_rid"), PropertyInfo(Variant::OBJECT, "object"),
                PropertyInfo(Variant::ARRAY, "ignore_subobjects"),
                PropertyInfo(Variant::ARRAY, "unignore_subobjects"),
                PropertyInfo(Variant::ARRAY, "ignore_subobjects_rid"),
                PropertyInfo(Variant::ARRAY, "unignore_subobjects_rid"),
                PropertyInfo(Variant::ARRAY, "visible_subobjects"),
                PropertyInfo(Variant::ARRAY, "invisible_subobjects")));

    ADD_SIGNAL(MethodInfo("exited_with_subnodes", PropertyInfo(Variant::RID, "object_rid"), PropertyInfo(Variant::OBJECT, "object"),
                PropertyInfo(Variant::ARRAY, "ignore_subobjects"),
                PropertyInfo(Variant::ARRAY, "unignore_subobjects"),
                PropertyInfo(Variant::ARRAY, "ignore_subobjects_rid"),
                PropertyInfo(Variant::ARRAY, "unignore_subobjects_rid"),
                PropertyInfo(Variant::ARRAY, "visible_subobjects"),
                PropertyInfo(Variant::ARRAY, "invisible_subobjects")));
    


}
#undef GETSET_GD3D
#undef ADDPROP_GD3D

bool GD3Dinterior_area::initialize()
{
    enter_ignore_subnodes.clear();
    enter_unignore_subnodes.clear();
    enter_visible_subnodes.clear();
    enter_invisible_subnodes.clear();

    enter_ignore_node = Object::cast_to<Node>(get_node<Node>(enter_ignore_node_path);
    if(enter_ignore_node != nullptr)
    {
        enter_ignore_subnodes = get_all_sub_physics_bodies(enter_ignore_node);
        enter_ignore_subnodes_as_rid = {};
        if(enter_ignore_subnodes.size()>0)
        {
            for (int64_t i = 0; i < enter_ignore_subnodes.size(); i++)
            {
                PhysicsBody3D* pb_child = cast_to<PhysicsBody3D>(enter_ignore_subnodes[i]);
                if (pb_child != nullptr)
                {
                    enter_ignore_subnodes_as_rid.append(pb_child->get_rid());
                }
            }
        }
    }
    enter_unignore_node = Object::cast_to<Node>(get_node<Node>(enter_unignore_node_path);
    if(enter_unignore_node != nullptr)
    {
        enter_unignore_subnodes = get_all_sub_physics_bodies(enter_unignore_node);
        enter_unignore_subnodes_as_rid = {};
        if (enter_unignore_subnodes.size() > 0)
        {
            for (int64_t i = 0; i < enter_unignore_subnodes.size(); i++)
            {
                PhysicsBody3D* pb_child = cast_to<PhysicsBody3D>(enter_unignore_subnodes[i]);
                if (pb_child != nullptr)
                {
                    enter_unignore_subnodes_as_rid.append(pb_child->get_rid());
                }
            }
        }
    }
    enter_visbible_node = Object::cast_to<Node>(get_node<Node>(enter_visible_node_path);
    if(enter_visbible_node != nullptr)
    {
        enter_visible_subnodes = get_all_sub_nodes(enter_visible_node);
    }
    enter_invisbible_node = Object::cast_to<Node>(get_node<Node>(enter_invisible_node_path);
    if (enter_invisbible_node != nullptr)
    {
        enter_invisible_subnodes = get_all_sub_nodes(enter_invisible_node);
    }
    return true;
}

void GD3Dinterior_area::uninitialize()
{
    enter_ignore_subnodes.clear();
    enter_unignore_subnodes.clear();
    enter_ignore_subnodes_as_rid.clear();
    enter_unignore_subnodes_as_rid.clear();
    enter_visible_subnodes.clear();
    enter_invisible_subnodes.clear();

    enter_ignore_node = nullptr;
    enter_unignore_node = nullptr;
    enter_visible_node = nullptr;
    enter_invisible_node = nullptr;
}
void GD3Dinterior_area::on_enter()
{
    WARN_PRINT("entered");
    emit_signal("entered_signal", get_rid(), this);
    emit_signal("entered_with_subnodes", get_rid(), this, enter_ignore_subnodes, enter_unignore_subnodes,
        enter_ignore_subnodes_as_rid, enter_unignore_subnodes_as_rid, enter_visible_subnodes, enter_invisible_subnodes);

}
void GD3Dinterior_area::on_exit()
{
    emit_signal("exited_signal", get_rid(), this);
    emit_signal("exited_with_subnodes", get_rid(), this, enter_ignore_subnodes, enter_unignore_subnodes,
        enter_ignore_subnodes_as_rid, enter_unignore_subnodes_as_rid, enter_visible_subnodes, enter_invisible_subnodes);
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

TypedArray<PhysicsBody3D> GD3Dinterior_area::get_enter_ignore_subnodes() { return enter_ignore_subnodes; }
TypedArray<PhysicsBody3D> GD3Dinterior_area::get_enter_unignore_subnodes() { return enter_unignore_subnodes; }
TypedArray<RID> GD3Dinterior_area::get_enter_ignore_subnodes_as_rid() { return enter_ignore_subnodes_as_rid; }
TypedArray<RID> GD3Dinterior_area::get_enter_unignore_subnodes_as_rid() { return enter_unignore_subnodes_as_rid; }
TypedArray<Node> GD3Dinterior_area::get_enter_visible_subnodes() { return enter_visible_subnodes; }
TypedArray<Node> GD3Dinterior_area::get_enter_invisible_subnodes() { return enter_invisible_subnodes; }

NodePath GD3Dinterior_area::get_enter_ignore_node_path() const { return enter_ignore_node_path;}
NodePath GD3Dinterior_area::get_enter_unignore_node_path() const { return enter_unignore_node_path;}
NodePath GD3Dinterior_area::get_enter_visible_node_path() const { return enter_visible_node_path;}
NodePath GD3Dinterior_area::get_enter_invisible_node_path() const { return enter_invisible_node_path;}

void GD3Dinterior_area::set_enter_ignore_node_path(const NodePath& path) { enter_ignore_node_path = path;}
void GD3Dinterior_area::set_enter_unignore_node_path(const NodePath& path) { enter_unignore_node_path = path;}
void GD3Dinterior_area::set_enter_visible_node_path(const NodePath& path) { enter_visible_node_path = path;}
void GD3Dinterior_area::set_enter_invisible_node_path(const NodePath& path) { enter_invisible_node_path = path;}


