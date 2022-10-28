#ifdef WIN32
#include <windows.h>
#endif
#ifndef GD3DVISUAL_OBSTACLE
#define GD3DVISUAL_OBSTACLE

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/global_constants.hpp>

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

	void init_obstacle();
	void uninit_obstacle();
	void obstacle_entered(uint32_t ignoremask);
	void obstacle_exited(uint32_t ignoremask);
	void obstacle_entered_char(uint32_t ignoremask);
	void obstacle_exited_char(uint32_t ignoremask);
	void make_invisible();
	void make_visible();
	

#define GETTERSETTER_GD3D(TYPE,VAR) void set_##VAR##(const TYPE##& set);\
                                            TYPE get_##VAR##() const
	GETTERSETTER_GD3D(bool, auto_ignore);
	GETTERSETTER_GD3D(bool, auto_invisible);
	GETTERSETTER_GD3D(StringName, shader_param);
	GETTERSETTER_GD3D(float, shader_param_min);
	GETTERSETTER_GD3D(float, shader_param_max);
	GETTERSETTER_GD3D(float, shader_duration);
#undef GETTERSETTER_GD3D
	TypedArray<GD3Dinterior_area> get_interior_area_parents(Node3D* nd);
};

#endif 

