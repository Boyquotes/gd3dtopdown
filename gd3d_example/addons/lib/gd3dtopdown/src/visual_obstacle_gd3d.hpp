#ifndef GD3DVISUAL_OBSTACLE
#define GD3DVISUAL_OBSTACLE
#ifdef WIN32
#include <windows.h>
#endif

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/static_body3d.hpp>

#include <godot_cpp/classes/project_settings.hpp>
#include "interior_area_gd3d.hpp"

using namespace godot;

class VisualObstacleGD3D : public StaticBody3D
{
	GDCLASS(VisualObstacleGD3D, StaticBody3D)

protected:
	void _notification(int p_what);
	static void _bind_methods();
private:
	uint32_t collision_layer;
	uint32_t ignoremask;
	bool is_invisible;
	bool entered_by_char;

public:
#include "helpers_gd3d.hpp"

	void obstacle_entered();
	void obstacle_exited();

};

#endif 

