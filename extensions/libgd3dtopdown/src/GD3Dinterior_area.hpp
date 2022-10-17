#ifdef WIN32
#include <windows.h>
#endif
#ifndef GD3DINTERIOR_AREA
#define GD3DINTERIOR_AREA

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/area3d.hpp>

#include <godot_cpp/classes/physics_body3d.hpp>
#include <godot_cpp/classes/node.hpp>

using namespace godot;

class GD3Dinterior_area : public Area3D
{
	GDCLASS(GD3Dinterior_area, Area3D);
	GD3Dinterior_area();
	~GD3Dinterior_area();

protected:
	static void _bind_methods();

private:

	Node* enter_ignore_node;
	Node* exit_ignore_node;
	Node* enter_unignore_node;
	Node* exit_unignore_node;
	Node* enter_visible_node;
	Node* exit_visible_node;
	Node* enter_invisible_node;
	Node* exit_invisible_node;

	TypedArray<PhysicsBody3D> enter_ignore_subnodes;
	TypedArray<PhysicsBody3D> exit_ignore_subnodes;
	TypedArray<PhysicsBody3D> enter_unignore_subnodes;
	TypedArray<PhysicsBody3D> exit_unignore_subnodes;
	TypedArray<Node> enter_visible_subnodes;
	TypedArray<Node> exit_visible_subnodes;
	TypedArray<Node> enter_invisible_subnodes;
	TypedArray<Node> exit_invisible_subnodes;

	NodePath enter_ignore_node_path;
	NodePath exit_ignore_node_path;
	NodePath enter_unignore_node_path;
	NodePath exit_unignore_node_path;
	NodePath enter_visible_node_path;
	NodePath exit_visible_node_path;
	NodePath enter_invisible_node_path;
	NodePath exit_invisible_node_path;

public:
	void on_enter_ignore();
	bool initialize();
	void uninitialize();

	TypedArray<Node> get_all_sub_nodes(Node* nd);
	TypedArray<PhysicsBody3D> get_all_sub_physics_bodies(Node* nd);

	NodePath get_enter_ignore_node_path() const;
	NodePath get_exit_ignore_node_path() const;
	NodePath get_enter_unignore_node_path() const;
	NodePath get_exit_unignore_node_path() const;

	NodePath get_enter_visible_node_path() const;
	NodePath get_exit_visible_node_path() const;
	NodePath get_enter_invisible_node_path() const;
	NodePath get_exit_invisible_node_path() const;

	void set_enter_ignore_node_path(const NodePath& path);
	void set_exit_ignore_node_path(const NodePath& path);
	void set_enter_unignore_node_path(const NodePath& path);
	void set_exit_unignore_node_path(const NodePath& path);

	void set_enter_visible_node_path(const NodePath& path);
	void set_exit_visible_node_path(const NodePath& path);
	void set_enter_invisible_node_path(const NodePath& path);
	void set_exit_invisible_node_path(const NodePath& path);

};



#endif