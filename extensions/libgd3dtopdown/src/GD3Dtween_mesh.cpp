#include "GD3Dtween_mesh.h"

void GD3Dtween_mesh::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("_tween_shader"), &GD3Dtween_mesh::_tween_shader);

	ClassDB::bind_method(D_METHOD("set_shader_param", "shader_param"), &GD3Dtween_mesh::set_shader_param);
	ClassDB::bind_method(D_METHOD("get_shader_param"), &GD3Dtween_mesh::get_shader_param);
	ClassDB::bind_method(D_METHOD("set_shader_param_min", "shader_param_min"), &GD3Dtween_mesh::set_shader_param_min);
	ClassDB::bind_method(D_METHOD("get_shader_param_min"), &GD3Dtween_mesh::get_shader_param_min);
	ClassDB::bind_method(D_METHOD("set_shader_param_max", "shader_param_max"), &GD3Dtween_mesh::set_shader_param_max);
	ClassDB::bind_method(D_METHOD("get_shader_param_max"), &GD3Dtween_mesh::get_shader_param_max);
	ClassDB::bind_method(D_METHOD("set_shader_duration", "shader_duration"), &GD3Dtween_mesh::set_shader_duration);
	ClassDB::bind_method(D_METHOD("get_shader_duration"), &GD3Dtween_mesh::get_shader_duration);

	ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "shader_param"), "set_shader_param", "get_shader_param");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "shader_param_min"), "set_shader_param_min", "get_shader_param_min");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "shader_param_max"), "set_shader_param_max", "get_shader_param_max");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "shader_duration"), "set_shader_duration", "get_shader_duration");
}
void GD3Dtween_mesh::_notification(int p_what)
{
	switch (p_what)
	{
		case NOTIFICATION_READY:
		{
			material = cast_to<ShaderMaterial>(get_mesh()->surface_get_material(0).ptr());
			if (!(material && material->get_shader_parameter(shader_param) != Variant()))
				material = nullptr;
			reset_tweens();
		}break;
		case NOTIFICATION_VOBSTACLE_INVISIBLE:
		{
			
			if (material)
				start_tween();
		}break;
		case NOTIFICATION_VOBSTACLE_VISIBLE:
		{
			if (material)
				reverse_tween();
		}break;
	}
}
void GD3Dtween_mesh::start_tween()
{
	double elapsed_time = 0;
	if (tw_visible->is_running())
		elapsed_time = tw_invisible->get_total_elapsed_time();

	tw_visible->stop();
	tw_invisible->stop();
	tw_invisible->custom_step(shader_duration - elapsed_time);
	tw_invisible->play();
	
}
void GD3Dtween_mesh::reverse_tween()
{
	double elapsed_time = 0;
	if (tw_invisible->is_running())
		elapsed_time = tw_invisible->get_total_elapsed_time();

	tw_invisible->stop();
	tw_visible->stop();
	tw_visible->custom_step(shader_duration - elapsed_time);
	tw_visible->play();
}
void GD3Dtween_mesh::reset_tweens()
{
	tw_invisible = this->create_tween();
	tw_visible = this->create_tween();
	tw_invisible->connect("finished", Callable(tw_invisible.ptr(), "stop"));
	tw_visible->connect("finished", Callable(tw_visible.ptr(), "stop"));
	tw_invisible->tween_method(Callable(this, "_tween_shader"), shader_param_min, shader_param_max, shader_duration);
	tw_visible->tween_method(Callable(this, "_tween_shader"), shader_param_max, shader_param_min, shader_duration);
	tw_invisible->stop();
	tw_visible->stop();
}
void GD3Dtween_mesh::_tween_shader(float progress)
{
	material->set_shader_parameter(shader_param, progress);
}

void GD3Dtween_mesh::set_shader_param(const StringName& set) {shader_param = set;}
StringName GD3Dtween_mesh::get_shader_param() const { return shader_param; }
void GD3Dtween_mesh::set_shader_param_min(const float set){ shader_param_min = set; }
float GD3Dtween_mesh::get_shader_param_min() const { return shader_param_min; }
void GD3Dtween_mesh::set_shader_param_max(const float set){ shader_param_max = set; }
float GD3Dtween_mesh::get_shader_param_max() const { return shader_param_max; }
void GD3Dtween_mesh::set_shader_duration(const float set){ shader_duration = set; }
float GD3Dtween_mesh::get_shader_duration() const { return shader_duration; }
