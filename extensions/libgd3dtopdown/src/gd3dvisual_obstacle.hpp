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
	bool locked;
	MeshInstance3D* visible_mesh;
	MeshInstance3D* shadow_mesh;
	Ref<ShaderMaterial> visible_material;

	uint32_t collision_layer;
	
	bool auto_invisible = false;
	bool auto_ignore = false;
	StringName shader_param = "";
	float shader_param_min = 0;
	float shader_param_max = 0;
	bool use_shader = false;
	bool is_invisible;
public:

	void initialize();
	void uninitialize();

	virtual void on_enter_obstacle(uint32_t ignoremask);
	virtual void on_exit_obstacle();
	void make_invisible();
	void make_visible();
	

#define GETTERSETTER_GD3D(TYPE,VAR) void set_##VAR##(const TYPE##& set);\
                                            TYPE get_##VAR##() const
	GETTERSETTER_GD3D(bool, locked);
	GETTERSETTER_GD3D(bool, auto_ignore);
	GETTERSETTER_GD3D(bool, auto_invisible);
	GETTERSETTER_GD3D(StringName, shader_param);
	GETTERSETTER_GD3D(float, shader_param_min);
	GETTERSETTER_GD3D(float, shader_param_max);
#undef GETTERSETTER_GD3D

};

#endif 

