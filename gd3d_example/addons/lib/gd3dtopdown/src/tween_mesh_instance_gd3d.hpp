#ifndef GD3DTWEEN_MESH
#define  GD3DTWEEN_MESH
#ifdef WIN32
#include <windows.h>
#endif

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/global_constants.hpp>

#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/material.hpp>
#include <godot_cpp/classes/shader_material.hpp>

#include <godot_cpp/variant/variant.hpp>

#include <godot_cpp/classes/tween.hpp>
#include <godot_cpp/classes/method_tweener.hpp>

using namespace godot;

class TweenMeshInstanceGD3D : public MeshInstance3D
{
	GDCLASS(TweenMeshInstanceGD3D, MeshInstance3D);

protected:

	static void _bind_methods();
	void _notification(int p_what);

	StringName shader_param = "";
	float shader_param_min = 0;
	float shader_param_max = 0;
	float shader_duration = 0;
	bool use_shader = false;

	ShaderMaterial* material;
	Ref<Tween> tw_invisible;
	Ref<Tween> tw_visible;

	void _tween_shader(float progress);

public:
#include "helpers_gd3d.hpp"

	void reset_tweens();

	void start_tween();
	void reverse_tween();

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