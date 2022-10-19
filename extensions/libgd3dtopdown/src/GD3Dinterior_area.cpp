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

    ClassDB::bind_method(D_METHOD("get_enter_ignore_bodies"), &GD3Dinterior_area::get_enter_ignore_bodies);
    ClassDB::bind_method(D_METHOD("get_enter_unignore_bodies"), &GD3Dinterior_area::get_enter_unignore_bodies);
    ClassDB::bind_method(D_METHOD("get_enter_ignore_bodies_as_rid"), &GD3Dinterior_area::get_enter_ignore_bodies);
    ClassDB::bind_method(D_METHOD("get_enter_unignore_bodies_as_rid"), &GD3Dinterior_area::get_enter_unignore_bodies);

    GETSET_GD3D(on_enter_ignore);
    GETSET_GD3D(on_enter_unignore);
    GETSET_GD3D(on_enter_visible);
    GETSET_GD3D(on_enter_invisible);
    GETSET_GD3D(enter_ignore_node_path);
    GETSET_GD3D(enter_unignore_node_path);
    GETSET_GD3D(enter_visible_node_path);
    GETSET_GD3D(enter_invisible_node_path);

    ADDPROP_GD3D(BOOL,on_enter_ignore);
    ADDPROP_GD3D(BOOL,on_enter_unignore);
    ADDPROP_GD3D(BOOL,on_enter_visible);
    ADDPROP_GD3D(BOOL,on_enter_invisible);
    ADDPROP_GD3D(NODE_PATH,enter_ignore_node_path);
    ADDPROP_GD3D(NODE_PATH,enter_unignore_node_path);
    ADDPROP_GD3D(NODE_PATH,enter_visible_node_path);
    ADDPROP_GD3D(NODE_PATH,enter_invisible_node_path);

    ADD_SIGNAL(MethodInfo("entered_signal", PropertyInfo(Variant::RID, "object_rid"), PropertyInfo(Variant::OBJECT, "object")));
    ADD_SIGNAL(MethodInfo("exited_signal", PropertyInfo(Variant::RID, "object_rid"), PropertyInfo(Variant::OBJECT, "object")));

}
#undef GETSET_GD3D
#undef ADDPROP_GD3D

bool GD3Dinterior_area::initialize()
{
    uninitialize();

    if (!enter_ignore_node_path.is_empty())
    {
        enter_ignore_node = Object::cast_to<Node>(get_node<Node>(enter_ignore_node_path));
    }
    if(enter_ignore_node != nullptr)
    {
        enter_ignore_bodies = get_all_sub_physics_bodies(enter_ignore_node);
        if(enter_ignore_bodies.size()>0)
        {
            for (int64_t i = 0; i < enter_ignore_bodies.size(); i++)
            {
                PhysicsBody3D* pb_child = cast_to<PhysicsBody3D>(enter_ignore_bodies[i]);
                if (pb_child == nullptr) continue;

                enter_ignore_bodies_as_rid.append(pb_child->get_rid());

                TypedArray<Node> pb_children = pb_child->get_children();

                if (pb_children.size() < 1) continue;

                for (int64_t j = 0; j < pb_children.size(); j++)
                {
                    CollisionShape3D* cs_child = cast_to<CollisionShape3D>(pb_children[j]);

                    if (cs_child == nullptr) continue;

                    enter_ignore_shapes.append(cs_child);
                }

            }
        }
    }
    if(!enter_unignore_node_path.is_empty())
    {
        enter_unignore_node = Object::cast_to<Node>(get_node<Node>(enter_unignore_node_path));
    }
    if(enter_unignore_node != nullptr)
    {
        enter_unignore_bodies = get_all_sub_physics_bodies(enter_unignore_node);
        if (enter_unignore_bodies.size() > 0)
        {
            for (int64_t i = 0; i < enter_unignore_bodies.size(); i++)
            {

                PhysicsBody3D* pb_child = cast_to<PhysicsBody3D>(enter_unignore_bodies[i]);

                if (pb_child == nullptr) continue;

                enter_unignore_bodies_as_rid.append(pb_child->get_rid());

                TypedArray<Node> pb_children = pb_child->get_children();

                if (pb_children.size() < 1) continue;

                for (int64_t j = 0; j < pb_children.size(); j++)
                {
                    CollisionShape3D* cs_child = cast_to<CollisionShape3D>(pb_children[j]);
                    
                    if(cs_child == nullptr) continue;

                    enter_unignore_shapes.append(cs_child);
                }
            }
        }
    }
    if(!enter_visible_node_path.is_empty())
    {
        enter_visible_node = Object::cast_to<Node3D>(get_node<Node>(enter_visible_node_path));
    }
    if (!enter_invisible_node_path.is_empty())
    {
        enter_invisible_node = Object::cast_to<Node3D>(get_node<Node>(enter_invisible_node_path));
    }
    return true;
    WARN_PRINT(String::num_int64(enter_ignore_shapes.size()));
}

void GD3Dinterior_area::uninitialize()
{
    enter_ignore_bodies.clear();
    enter_unignore_bodies.clear();
    enter_ignore_shapes.clear();
    enter_unignore_shapes.clear();
    enter_ignore_bodies_as_rid.clear();
    enter_unignore_bodies_as_rid.clear();

    enter_ignore_node = nullptr;
    enter_unignore_node = nullptr;
    enter_visible_node = nullptr;
    enter_invisible_node = nullptr;
}
void GD3Dinterior_area::on_enter()
{
    if(on_enter_ignore && enter_ignore_shapes.size()>0)
    {
        for (int64_t i = 0; i < enter_ignore_shapes.size(); i++)
        {
            CollisionShape3D* n_child = cast_to<CollisionShape3D>(enter_ignore_shapes[i]);
           
            if (n_child == nullptr) continue;
            WARN_PRINT("a");
            n_child->call_deferred("set_disabled", true);

        }
    }
    if (on_enter_unignore && enter_unignore_shapes.size() > 0)
    {
        for (int64_t i = 0; i < enter_unignore_shapes.size(); i++)
        {
            CollisionShape3D* n_child = cast_to<CollisionShape3D>(enter_unignore_shapes[i]);

            if (n_child == nullptr) continue;
            n_child->call_deferred("set_disabled", false);

        }
    }
    if(on_enter_invisible && enter_invisible_node != nullptr)
    {
        enter_invisible_node->set_visible(false);
        WARN_PRINT("b");

    }
    if (on_enter_visible && enter_visible_node != nullptr)
    {
        enter_visible_node->set_visible(true);
    }
    emit_signal("entered_signal", get_rid(), this);
}
void GD3Dinterior_area::on_exit()
{
    

    if (on_enter_ignore && enter_ignore_shapes.size() > 0)
    {
        for (int64_t i = 0; i < enter_ignore_shapes.size(); i++)
        {
            CollisionShape3D* n_child = cast_to<CollisionShape3D>(enter_ignore_shapes[i]);

            if (n_child == nullptr) continue;
            n_child->call_deferred("set_disabled", false);
        }
    }
    if (on_enter_unignore && enter_unignore_shapes.size() > 0)
    {
        for (int64_t i = 0; i < enter_unignore_shapes.size(); i++)
        {
            CollisionShape3D* n_child = cast_to<CollisionShape3D>(enter_unignore_shapes[i]);

            if (n_child == nullptr) continue;
            n_child->call_deferred("set_disabled", true);
        }
    }
    if (on_enter_visible && enter_visible_node != nullptr)
    {
        enter_visible_node->set_visible(false);
    }
    if (on_enter_invisible && enter_invisible_node != nullptr)
    {
        enter_invisible_node->set_visible(true);
    }
    emit_signal("exited_signal", get_rid(), this);
}
//Other functions
TypedArray<PhysicsBody3D> GD3Dinterior_area::get_all_sub_physics_bodies(Node* nd)
{
    TypedArray<PhysicsBody3D> pb_array = {};
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
TypedArray<Node> GD3Dinterior_area::get_all_sub_nodes( Node * nd)
{
    TypedArray<Node> cols = nd->get_children();
    TypedArray<Node> n_arr = {};
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
Node* GD3Dinterior_area::get_enter_unignore_node() { return enter_unignore_node; }
Node3D* GD3Dinterior_area::get_enter_visible_node() { return enter_visible_node; }
Node3D* GD3Dinterior_area::get_enter_invisible_node() { return enter_invisible_node;}

TypedArray<PhysicsBody3D> GD3Dinterior_area::get_enter_ignore_bodies() { return enter_ignore_bodies; }
TypedArray<PhysicsBody3D> GD3Dinterior_area::get_enter_unignore_bodies() { return enter_unignore_bodies; }
TypedArray<CollisionShape3D> GD3Dinterior_area::get_enter_ignore_shapes() { return enter_unignore_shapes; }
TypedArray<CollisionShape3D> GD3Dinterior_area::get_enter_unignore_shapes() { return enter_ignore_shapes; }
TypedArray<RID> GD3Dinterior_area::get_enter_ignore_bodies_as_rid() { return enter_ignore_bodies_as_rid; }
TypedArray<RID> GD3Dinterior_area::get_enter_unignore_bodies_as_rid() { return enter_unignore_bodies_as_rid; }

bool GD3Dinterior_area::get_on_enter_ignore() const { return on_enter_ignore; }
bool GD3Dinterior_area::get_on_enter_unignore()  const{ return on_enter_unignore; }
bool GD3Dinterior_area::get_on_enter_visible() const { return on_enter_visible; }
bool GD3Dinterior_area::get_on_enter_invisible() const { return on_enter_invisible; }

NodePath GD3Dinterior_area::get_enter_ignore_node_path() const { return enter_ignore_node_path;}
NodePath GD3Dinterior_area::get_enter_unignore_node_path() const { return enter_unignore_node_path;}
NodePath GD3Dinterior_area::get_enter_visible_node_path() const { return enter_visible_node_path;}
NodePath GD3Dinterior_area::get_enter_invisible_node_path() const { return enter_invisible_node_path;}

void GD3Dinterior_area::set_on_enter_ignore(const bool var) { on_enter_ignore = var; }
void GD3Dinterior_area::set_on_enter_unignore(const bool var) { on_enter_unignore = var; }
void GD3Dinterior_area::set_on_enter_visible(const bool var) { on_enter_visible = var; }
void GD3Dinterior_area::set_on_enter_invisible(const bool var) { on_enter_invisible = var; }

void GD3Dinterior_area::set_enter_ignore_node_path(const NodePath& path) { enter_ignore_node_path = path;}
void GD3Dinterior_area::set_enter_unignore_node_path(const NodePath& path) { enter_unignore_node_path = path;}
void GD3Dinterior_area::set_enter_visible_node_path(const NodePath& path) { enter_visible_node_path = path;}
void GD3Dinterior_area::set_enter_invisible_node_path(const NodePath& path) { enter_invisible_node_path = path;}


