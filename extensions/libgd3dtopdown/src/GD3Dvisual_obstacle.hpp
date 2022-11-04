#ifdef WIN32
#include <windows.h>
#endif
#ifndef GD3DVISUAL_OBSTACLE
#define GD3DVISUAL_OBSTACLE

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/global_constants.hpp>

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/static_body3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/geometry_instance3d.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/material.hpp>
#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/classes/tween.hpp>
#include <godot_cpp/classes/method_tweener.hpp>

#include <GD3Dinterior_area.hpp>

using namespace godot;

class GD3Dvisual_obstacle : public StaticBody3D
{
	GDCLASS(GD3Dvisual_obstacle, StaticBody3D)

	GD3Dvisual_obstacle();
	~GD3Dvisual_obstacle();

protected:
	static void _bind_methods();
private:
	bool initialized;
	MeshInstance3D* visible_mesh;
	MeshInstance3D* shadow_mesh;
	Ref<ShaderMaterial> visible_material;

	uint32_t collision_layer;
	
	bool auto_invisible = false;
	bool auto_ignore = false;
	bool generate_shadow_mesh = true;
	StringName shader_param = "";
	float shader_param_min = 0;
	float shader_param_max = 0;
	float shader_duration = 0;
	bool use_shader = false;
	bool is_invisible;
	bool entered_by_char;
	TypedArray<GD3Dinterior_area> parent_interior_areas = {};
	bool under_area;
	bool under_multi_area;

	Ref<Tween> tw_invisible;
	Ref<Tween> tw_visible;

	void _invisible_shader_tween(float progress);
	void _visible_shader_tween(float progress);

public:

	virtual void _ready() override;
	virtual void _exit_tree() override;

	void init_obstacle();
	void uninit_obstacle();
	void obstacle_entered(uint32_t ignoremask);
	void obstacle_exited(uint32_t ignoremask);
	void obstacle_entered_char(uint32_t ignoremask);
	void obstacle_exited_char(uint32_t ignoremask);
	void make_invisible();
	void make_visible();
	TypedArray<GD3Dinterior_area> get_interior_area_parents(Node3D* nd);
	
	//Setter and getters
	void set_auto_ignore(const bool set);
	bool get_auto_ignore() const;
	void set_auto_invisible(const bool set);
	bool get_auto_invisible() const;
	void set_generate_shadow_mesh(const bool set);
	bool get_generate_shadow_mesh() const; 
	void set_shader_param(const StringName& set);
	StringName get_shader_param() const;
	void set_shader_param_min(const float set);
	float get_shader_param_min() const;
	void set_shader_param_max(const float set);
	float get_shader_param_max() const;
	void set_shader_duration(const float set);
	float get_shader_duration() const;
};

#endif 

