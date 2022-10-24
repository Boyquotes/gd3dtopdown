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
	
	ClassDB::bind_method(D_METHOD("on_enter_obstacle"), &GD3Dvisual_obstacle::on_enter_obstacle);
	ClassDB::bind_method(D_METHOD("on_exit_obstacle"), &GD3Dvisual_obstacle::on_exit_obstacle);

	GETSET_GD3D(locked);
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
	
	visible_mesh = nullptr;
	shadow_mesh = nullptr;
	if(!visible_material.is_null()) visible_material.unref();
	is_invisible = false;
	locked = false;
	if (!auto_invisible) return;

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
	initialized = true;
}
void GD3Dvisual_obstacle::uninitialize()
{
	if (shadow_mesh != nullptr) memfree(shadow_mesh);
	if (!visible_material.is_null()) visible_material.unref();
	shadow_mesh = nullptr;
	visible_mesh = nullptr;
	initialized = false;
}
void GD3Dvisual_obstacle::on_enter_obstacle(uint32_t ignoremask)
{
	collision_layer = get_collision_layer();

	if (auto_invisible)  make_invisible();
	if (auto_ignore) set_collision_layer((collision_layer & ignoremask) ^ collision_layer);
	emit_signal("visual_disappear_signal", get_rid(), this);
}
void GD3Dvisual_obstacle::on_exit_obstacle()
{

	if (auto_invisible) make_visible();
	if(auto_ignore) set_collision_layer(collision_layer);
	
	emit_signal("visual_appear_signal", get_rid(), this);
}
void GD3Dvisual_obstacle::make_invisible()
{
	
	if (!initialized || is_invisible || !auto_invisible || locked) return;
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
	
	if (!initialized || !is_invisible || !auto_invisible || locked ) return;
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
GETTERSETTER_GD3D(bool, locked);
GETTERSETTER_GD3D(bool, auto_ignore);
GETTERSETTER_GD3D(bool, auto_invisible);
GETTERSETTER_GD3D(StringName, shader_param);
GETTERSETTER_GD3D(float, shader_param_min);
GETTERSETTER_GD3D(float, shader_param_max);

#undef GETTERSETTER_GD3D