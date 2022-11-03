#include "GD3Dvisual_obstacle.hpp"

GD3Dvisual_obstacle::GD3Dvisual_obstacle()
{
	initialized = false;
	visible_mesh = nullptr;
	shadow_mesh = nullptr;
}

GD3Dvisual_obstacle::~GD3Dvisual_obstacle()
{
	
}
#define GETSET_GD3D(FUNC) ClassDB::bind_method(D_METHOD("get_"#FUNC), &GD3Dvisual_obstacle::get_##FUNC);\
                        ClassDB::bind_method(D_METHOD("set_"#FUNC, #FUNC), &GD3Dvisual_obstacle::set_##FUNC)
#define ADDPROP_GD3D(TYPE,PROP) ADD_PROPERTY(PropertyInfo(Variant::TYPE, #PROP), "set_"#PROP, "get_"#PROP)
void GD3Dvisual_obstacle::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("init_obstacle"), &GD3Dvisual_obstacle::init_obstacle);
	ClassDB::bind_method(D_METHOD("uninit_obstacle"), &GD3Dvisual_obstacle::uninit_obstacle);
	ClassDB::bind_method(D_METHOD("obstacle_entered"), &GD3Dvisual_obstacle::obstacle_entered);
	ClassDB::bind_method(D_METHOD("obstacle_entered_char"), &GD3Dvisual_obstacle::obstacle_entered_char);
	ClassDB::bind_method(D_METHOD("obstacle_exited"), &GD3Dvisual_obstacle::obstacle_exited);
	ClassDB::bind_method(D_METHOD("obstacle_exited_char"), &GD3Dvisual_obstacle::obstacle_exited_char);

	GETSET_GD3D(auto_ignore);
	GETSET_GD3D(auto_invisible);
	GETSET_GD3D(shader_param);
	GETSET_GD3D(shader_param_min);
	GETSET_GD3D(shader_param_max);
	GETSET_GD3D(shader_duration);
	
	ADDPROP_GD3D(BOOL, auto_ignore);
	ADDPROP_GD3D(BOOL, auto_invisible);
	ADDPROP_GD3D(STRING_NAME, shader_param);
	ADDPROP_GD3D(FLOAT, shader_param_min);
	ADDPROP_GD3D(FLOAT, shader_param_max);
	ADDPROP_GD3D(FLOAT, shader_duration);

	ADD_SIGNAL(MethodInfo("visual_disappear_signal", PropertyInfo(Variant::OBJECT, "object")));
	ADD_SIGNAL(MethodInfo("visual_appear_signal", PropertyInfo(Variant::OBJECT, "object")));

	ClassDB::bind_method(D_METHOD("_invisible_shader_tween"), &GD3Dvisual_obstacle::_invisible_shader_tween);
	ClassDB::bind_method(D_METHOD("_visible_shader_tween"), &GD3Dvisual_obstacle::_visible_shader_tween);
}
#undef GETSET_GD3D
#undef ADDPROP_GD3D

void GD3Dvisual_obstacle::_ready()
{
	if (Engine::get_singleton()->is_editor_hint()) return;
	init_obstacle();
}
void GD3Dvisual_obstacle::_exit_tree()
{
	uninit_obstacle();
}
#ifdef DEBUG_ENABLED
#define DEBUG_WARN_GD3D(msg) WARN_PRINT(msg)
#else
#define DEBUG_WARN_GD3D(msg) 
#endif
void GD3Dvisual_obstacle::init_obstacle()
{
	if (initialized) return;
	initialized = true;

	entered_by_char = false;
	under_area = false;
	under_multi_area = false;
	is_invisible = false;

	if(!visible_material.is_null()) visible_material.unref();

	collision_layer = get_collision_layer();

	if (!auto_invisible) return;

#pragma region create_shadow_mesh
	
	TypedArray<Node> cols = get_children();
	if (cols.size() < 1) return;
	for (int64_t i = 0; i < cols.size(); i++)
	{
		MeshInstance3D* n_child = cast_to<MeshInstance3D>(cols[i]);

		if (n_child != nullptr) visible_mesh = n_child;
	}
	if(visible_mesh != nullptr && auto_invisible)
	{
		visible_material = visible_mesh->get_mesh()->surface_get_material(0);
		if (!visible_material.is_valid())
		{
			use_shader = false;
			DEBUG_WARN_GD3D(" " + godot::String(this->get_name()) + ": Could not get valid shader material in meshInstance, will use use set_visible()");
		}
		else if(!visible_material.is_valid() && visible_material->get_shader_parameter(shader_param) == Variant())
		{
			use_shader = false;
			DEBUG_WARN_GD3D(" "+ godot::String(this->get_name()) + ": Could not get specified shader parameter, will use set_visible()");
		}else
		{
			use_shader = true;
			tw_invisible = this->create_tween();
			tw_visible = this->create_tween();
			tw_invisible->connect("finished", Callable(tw_invisible.ptr(), "stop"));//Prevents the tween becoming disabled when it finishes
			tw_visible->connect("finished", Callable(tw_visible.ptr(), "stop"));
			tw_invisible->tween_method(Callable(this, "_invisible_shader_tween"), shader_param_min, shader_param_max, shader_duration);
			tw_visible->tween_method(Callable(this, "_visible_shader_tween"), shader_param_max, shader_param_min, shader_duration);
			tw_invisible->stop();
			tw_visible->stop();
		}
		
		shadow_mesh = memnew(MeshInstance3D);
		add_child(shadow_mesh);
		shadow_mesh->set_owner(this);
		shadow_mesh->set_mesh(visible_mesh->get_mesh());
		shadow_mesh->set_material_override(memnew(Material));
		visible_mesh->set_cast_shadows_setting(GeometryInstance3D::SHADOW_CASTING_SETTING_OFF);
		shadow_mesh->set_cast_shadows_setting(GeometryInstance3D::SHADOW_CASTING_SETTING_SHADOWS_ONLY);
	}
#pragma endregion create_shadow_mesh
	
	parent_interior_areas = get_interior_area_parents(this);
	
	if (parent_interior_areas.size() > 0) 	under_area = true;
	
	if (parent_interior_areas.size() > 1)
	{
		under_multi_area = true;
	}
	
	for (int64_t i = 0; i < parent_interior_areas.size(); i++)
	{
		GD3Dinterior_area* area = cast_to<GD3Dinterior_area>(parent_interior_areas[i]);
		area->connect("entered_signal_mask", Callable(this, "obstacle_entered"));
		area->connect("exited_signal_mask", Callable(this, "obstacle_exited"));
	}
}
#undef DEBUG_WARN_GD3D
TypedArray<GD3Dinterior_area> GD3Dvisual_obstacle::get_interior_area_parents(Node3D* nd)
{
	TypedArray<GD3Dinterior_area> n_arr = {};
	Node3D* n_parent = nd->get_parent_node_3d();
	
	if (n_parent == nullptr) return n_arr;

	GD3Dinterior_area* area = dynamic_cast<GD3Dinterior_area*>(n_parent);

	if(area != nullptr) n_arr.append(area);

	n_arr.append_array(get_interior_area_parents(n_parent));
	
	return n_arr;
}
void GD3Dvisual_obstacle::uninit_obstacle()
{
	if (!initialized) return;
	initialized = false;
	
	if (shadow_mesh != nullptr) memdelete(shadow_mesh);
	if (!visible_material.is_null()) visible_material.unref();

	if (parent_interior_areas.size() > 0)
	{
		for (int64_t i = 0; i < parent_interior_areas.size(); i++)
		{
			GD3Dinterior_area* area = cast_to<GD3Dinterior_area>(parent_interior_areas[i]);
			if(area->is_connected("entered_signal_mask", Callable(this, "obstacle_entered")))
				area->disconnect("entered_signal_mask", Callable(this, "obstacle_entered"));
			if (area->is_connected("exited_signal_mask", Callable(this, "obstacle_exited")))
				area->disconnect("exited_signal_mask", Callable(this, "obstacle_exited"));
		}
	}

	parent_interior_areas.clear();
	shadow_mesh = nullptr;
	visible_mesh = nullptr;
}
void GD3Dvisual_obstacle::obstacle_entered_char(uint32_t ignoremask)
{
	this->obstacle_entered(ignoremask);
	entered_by_char = true;
}
void GD3Dvisual_obstacle::obstacle_exited_char(uint32_t ignoremask)
{
	entered_by_char = false;
	this->obstacle_exited(ignoremask);
}
void GD3Dvisual_obstacle::obstacle_entered(uint32_t ignoremask)
{
	if (entered_by_char) return;

	if (auto_invisible) make_invisible();
	
	if (auto_ignore) set_collision_layer((collision_layer & ignoremask) ^ collision_layer);
	emit_signal("visual_disappear_signal", get_rid(), this);
}
void GD3Dvisual_obstacle::obstacle_exited(uint32_t ignoremask)
{
	if (entered_by_char) return;
	if (!under_area) {
		if (auto_invisible) make_visible();
		if (auto_ignore) set_collision_layer(collision_layer);
		emit_signal("visual_appear_signal", get_rid(), this);
		return;
	}
	else if (!under_multi_area)
	{
		if (cast_to<GD3Dinterior_area>(parent_interior_areas[0])->is_entered()) return;
		if (auto_invisible) make_visible();
		if (auto_ignore) set_collision_layer(collision_layer);
		emit_signal("visual_appear_signal", get_rid(), this);
		return;
	}
	else
	{
		bool should_invisible = false;
		for (int64_t i = 0; i < parent_interior_areas.size(); i++)
		{
			should_invisible = cast_to<GD3Dinterior_area>(parent_interior_areas[i])->is_entered(); //<--Safe to call directly since it was pushed to a typedarray
			if (should_invisible) break;
		}
		if (should_invisible) return;
		if (auto_invisible) make_visible();
		if (auto_ignore) set_collision_layer(collision_layer);
		emit_signal("visual_appear_signal", get_rid(), this);
		return;
	}
	return;
}

void GD3Dvisual_obstacle::make_invisible()
{
	if (!initialized || is_invisible) return;
	is_invisible = true;

	if (use_shader)
	{
		double elapsed_time = 0;
		if (tw_visible->is_running())
			elapsed_time = tw_invisible->get_total_elapsed_time();

		tw_visible->stop();
		tw_invisible->stop();
		tw_invisible->custom_step(shader_duration - elapsed_time);
		tw_invisible->play();

	}
	else
	{
		visible_mesh->set_visible(false);
	}

}
void GD3Dvisual_obstacle::make_visible()
{
	if (!initialized || !is_invisible) return;
	is_invisible = false;
	if (use_shader) 
	{
		double elapsed_time = 0;
		if(tw_invisible->is_running())
			elapsed_time = tw_invisible->get_total_elapsed_time();

		tw_invisible->stop();
		tw_visible->stop();
		tw_visible->custom_step(shader_duration - elapsed_time);
		tw_visible->play();
	}
	else
	{
		visible_mesh->set_visible(true);
	}
}
void GD3Dvisual_obstacle::_invisible_shader_tween(float progress)
{
	visible_material->set_shader_parameter(shader_param, progress);
}
void GD3Dvisual_obstacle::_visible_shader_tween(float progress)
{
	visible_material->set_shader_parameter(shader_param, progress);
}

void GD3Dvisual_obstacle::set_auto_ignore(const bool set) { auto_ignore = set; }
bool GD3Dvisual_obstacle::get_auto_ignore() const { return auto_ignore; }
void GD3Dvisual_obstacle::set_auto_invisible(const bool set) { auto_invisible = set; }
bool GD3Dvisual_obstacle::get_auto_invisible() const {return auto_invisible;}
void GD3Dvisual_obstacle::set_shader_param(const StringName& set) { shader_param = set; }
StringName GD3Dvisual_obstacle::get_shader_param() const {return shader_param;}
void GD3Dvisual_obstacle::set_shader_param_min(const float set) { shader_param_min = set; }
float GD3Dvisual_obstacle::get_shader_param_min() const {return shader_param_min;}
void GD3Dvisual_obstacle::set_shader_param_max(const float set) { shader_param_max = set; }
float GD3Dvisual_obstacle::get_shader_param_max() const {return shader_param_max;}
void GD3Dvisual_obstacle::set_shader_duration(const float set) { shader_duration = set; }
float GD3Dvisual_obstacle::get_shader_duration( ) const { return shader_duration; }


