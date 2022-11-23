#ifndef GD3DVISUAL_DETECTOR
#define GD3DVISUAL_DETECTOR
#ifdef WIN32
#include <windows.h>
#endif

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/area3d.hpp>

using namespace godot;

class GD3Dvisual_detector : public Area3D
{
	GDCLASS(GD3Dvisual_detector, Area3D);

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
#include"GD3Dhelpers.hpp"
	
public:
	void visual_obstacle_entered(Object* body);
	
	void visual_obstacle_exited(Object* body);
	
};

#endif