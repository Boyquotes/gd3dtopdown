#include "GD3Dvisual_obstacle.hpp"

GD3Dvisual_obstacle::GD3Dvisual_obstacle()
{
	initialized = false;
}

GD3Dvisual_obstacle::~GD3Dvisual_obstacle()
{
	
}
#define GETSET_GD3D(FUNC) ClassDB::bind_method(D_METHOD("get_"#FUNC), &GD3Dvisual_obstacle::get_##FUNC);\
                        ClassDB::bind_method(D_METHOD("set_"#FUNC, #FUNC), &GD3Dvisual_obstacle::set_##FUNC)
#define ADDPROP_GD3D(TYPE,PROP) ADD_PROPERTY(PropertyInfo(Variant::##TYPE, #PROP), "set_"#PROP, "get_"#PROP)
void GD3Dvisual_obstacle::_bind_methods()
{
	//Signals
	ClassDB::bind_method(D_METHOD("initialize"), &GD3Dvisual_obstacle::initialize);
	ClassDB::bind_method(D_METHOD("uninitialize"), &GD3Dvisual_obstacle::uninitialize);
	
	ClassDB::bind_method(D_METHOD("obstacle_entered"), &GD3Dvisual_obstacle::obstacle_entered);
	ClassDB::bind_method(D_METHOD("obstacle_exited"), &GD3Dvisual_obstacle::obstacle_exited);

	GETSET_GD3D(auto_ignore);
	GETSET_GD3D(auto_invisible);
	GETSET_GD3D(shader_param);
	GETSET_GD3D(shader_param_min);
	GETSET_GD3D(shader_param_max);

	ADDPROP_GD3D(BOOL, auto_ignore);
	ADDPROP_GD3D(BOOL, auto_invisible);
	ADDPROP_GD3D(STRING_NAME, shader_param);
	ADDPROP_GD3D(FLOAT, shader_param_min);
	ADDPROP_GD3D(FLOAT, shader_param_max);

	ADD_SIGNAL(MethodInfo("visual_disappear_signal", PropertyInfo(Variant::OBJECT, "object")));
	ADD_SIGNAL(MethodInfo("visual_appear_signal", PropertyInfo(Variant::OBJECT, "object")));
}
#undef GETSET_GD3D
#undef ADDPROP_GD3D

void GD3Dvisual_obstacle::initialize()
{
	if (initialized) return;
	under_area = false;
	under_multi_area = false;
	visible_mesh = nullptr;
	shadow_mesh = nullptr;
	if(!visible_material.is_null()) visible_material.unref();
	is_invisible = false;
	if (!auto_invisible) return;

	collision_layer = get_collision_layer();

#pragma region create_shadow_mesh
	TypedArray<Node> cols = get_children();
	if (cols.size() < 1) return;
	for (int64_t i = 0; i < cols.size(); i++)
	{
		MeshInstance3D* n_child = cast_to<MeshInstance3D>(cols[i]);
		if (n_child == nullptr) continue;
		visible_mesh = n_child;
	}
	if(visible_mesh != nullptr && auto_invisible)
	{
		visible_material = visible_mesh->get_mesh()->surface_get_material(0);
		if (!visible_material.is_valid())
		{
			use_shader = false;
#ifdef DEBUG
			WARN_PRINT("Could not get valid shader material in meshInstance, will use use set_visible()")
#endif 
		}
		else if(!visible_material.is_valid() && visible_material->get_shader_parameter(shader_param) == Variant())
		{
			use_shader = false;
#ifdef DEBUG
			WARN_PRINT("Could not get specified shader parameter, will use use set_visible()")
#endif 
		}else
		{
			use_shader = true;
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
		WARN_PRINT(get_name());
		under_multi_area = true;
	}
	
	for (int64_t i = 0; i < parent_interior_areas.size(); i++)
	{
		GD3Dinterior_area* area = cast_to<GD3Dinterior_area>(parent_interior_areas[i]);
		area->connect("entered_signal_mask", Callable(this, "obstacle_entered"));
		area->connect("exited_signal_mask", Callable(this, "obstacle_exited"));

	}
	initialized = true;
}
TypedArray<GD3Dinterior_area> GD3Dvisual_obstacle::get_interior_area_parents(Node3D* nd)
{
	TypedArray<GD3Dinterior_area> n_arr = {};
	Node3D* n_parent = nd->get_parent_node_3d();
	
	if (n_parent == nullptr) return n_arr;
	GD3Dinterior_area* area = cast_to<GD3Dinterior_area>(n_parent);
	if(area != nullptr )
		if(area->get_class() == "GD3Dinterior_area")
			n_arr.append(area);

	n_arr.append_array(get_interior_area_parents(n_parent));
	
	return n_arr;
}
void GD3Dvisual_obstacle::uninitialize()
{
	if (shadow_mesh != nullptr) memfree(shadow_mesh);
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
	initialized = false;
}

void GD3Dvisual_obstacle::obstacle_entered(uint32_t ignoremask)
{
	if (auto_invisible) make_invisible();
	
	if (auto_ignore) set_collision_layer((collision_layer & ignoremask) ^ collision_layer);
	emit_signal("visual_disappear_signal", get_rid(), this);
}
void GD3Dvisual_obstacle::obstacle_exited(uint32_t ignoremask)
{
	
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
		WARN_PRINT("Exited");
		bool should_invisible = false;
		for (int64_t i = 0; i < parent_interior_areas.size(); i++)
		{
			should_invisible = cast_to<GD3Dinterior_area>(parent_interior_areas[i])->is_entered();
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
		visible_material->set_shader_parameter(shader_param, shader_param_max);
	}
	else
	{
		visible_mesh->set_visible(false);
	}

}
void GD3Dvisual_obstacle::make_visible()
{
	if (under_multi_area) WARN_PRINT("CalledInv");
	if (!initialized || !is_invisible) return;
	is_invisible = false;
	if (use_shader) 
	{
		visible_material->set_shader_parameter(shader_param, shader_param_min);
	}
	else
	{
		visible_mesh->set_visible(true);
	}
}

#define GETTERSETTER_GD3D(TYPE,VAR) void GD3Dvisual_obstacle::set_##VAR##(const TYPE##& set) { VAR = set;}\
                                            TYPE GD3Dvisual_obstacle::get_##VAR##() const {return VAR ;}
GETTERSETTER_GD3D(bool, auto_ignore);
GETTERSETTER_GD3D(bool, auto_invisible);
GETTERSETTER_GD3D(StringName, shader_param);
GETTERSETTER_GD3D(float, shader_param_min);
GETTERSETTER_GD3D(float, shader_param_max);

#undef GETTERSETTER_GD3D