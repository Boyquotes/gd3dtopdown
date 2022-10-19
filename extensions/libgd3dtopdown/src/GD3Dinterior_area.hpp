#ifdef WIN32
#include <windows.h>
#endif
#ifndef GD3DINTERIOR_AREA
#define GD3DINTERIOR_AREA

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/area3d.hpp>

#include <godot_cpp/classes/physics_body3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node3d.hpp>

using namespace godot;

class GD3Dinterior_area : public Area3D
{
	GDCLASS(GD3Dinterior_area, Area3D);
	GD3Dinterior_area();
	~GD3Dinterior_area();

protected:
	static void _bind_methods();

private:

	bool on_enter_ignore = true;
	bool on_enter_unignore = true;
	bool on_enter_visible = false;
	bool on_enter_invisible = false;

	Node* enter_ignore_node;
	Node* enter_unignore_node;
	Node3D* enter_visible_node;
	Node3D* enter_invisible_node;

	TypedArray<PhysicsBody3D> enter_ignore_bodies;
	TypedArray<PhysicsBody3D> enter_unignore_bodies;
	TypedArray<CollisionShape3D> enter_ignore_shapes;
	TypedArray<CollisionShape3D> enter_unignore_shapes;
	TypedArray<RID> enter_ignore_bodies_as_rid;
	TypedArray<RID> enter_unignore_bodies_as_rid;


	NodePath enter_ignore_node_path;
	NodePath enter_unignore_node_path;
	NodePath enter_visible_node_path;
	NodePath enter_invisible_node_path;

public:
	
	bool initialize();
	void uninitialize();
	void on_enter();
	void on_exit();


	TypedArray<PhysicsBody3D> get_enter_ignore_bodies();
	TypedArray<PhysicsBody3D> get_enter_unignore_bodies();
	TypedArray<CollisionShape3D> get_enter_ignore_shapes();
	TypedArray<CollisionShape3D> get_enter_unignore_shapes();
	TypedArray<RID> get_enter_ignore_bodies_as_rid();
	TypedArray<RID> get_enter_unignore_bodies_as_rid();

	Node* get_enter_ignore_node();
	Node* get_enter_unignore_node();
	Node3D* get_enter_visible_node();
	Node3D* get_enter_invisible_node();


	bool get_on_enter_ignore() const;
	bool get_on_enter_unignore() const;
	bool get_on_enter_visible() const;
	bool get_on_enter_invisible() const;

	NodePath get_enter_ignore_node_path() const;
	NodePath get_enter_unignore_node_path() const;
	NodePath get_enter_visible_node_path() const;
	NodePath get_enter_invisible_node_path() const;

	void set_on_enter_ignore(const bool var);
	void set_on_enter_unignore(const bool var);
	void set_on_enter_visible(const bool var);
	void set_on_enter_invisible(const bool var );

	void set_enter_ignore_node_path(const NodePath& path);
	void set_enter_unignore_node_path(const NodePath& path);
	void set_enter_visible_node_path(const NodePath& path);
	void set_enter_invisible_node_path(const NodePath& path);

	//Internal functions
	TypedArray<Node> get_all_sub_nodes(Node* nd);
	TypedArray<PhysicsBody3D> get_all_sub_physics_bodies(Node* nd);
};



#endif