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
	static void _bind_methods()
	{
		ClassDB::bind_method(D_METHOD("visual_obstacle_entered"), &GD3Dvisual_detector::visual_obstacle_entered);
		ClassDB::bind_method(D_METHOD("visual_obstacle_exited"), &GD3Dvisual_detector::visual_obstacle_exited);
	}

public:
#include"GD3Dhelpers.hpp"
protected:
	void _notification(int p_what)
	{
		switch (p_what)
		{
		case NOTIFICATION_ENTER_TREE:
		{
			if (!is_connected("body_entered", Callable(this, "visual_obstacle_entered")))
				connect("body_entered", Callable(this, "visual_obstacle_entered"));
			if (!is_connected("body_exited", Callable(this, "visual_obstacle_exited")))
				connect("body_exited", Callable(this, "visual_obstacle_exited"));
		}break;
		case NOTIFICATION_EXIT_TREE:
		{
			if (is_connected("body_entered", Callable(this, "visual_obstacle_entered")))
				disconnect("body_entered", Callable(this, "visual_obstacle_entered"));
			if (is_connected("body_exited", Callable(this, "visual_obstacle_exited")))
				disconnect("body_exited", Callable(this, "visual_obstacle_exited"));
		}break;
		}
	}
public:
	void visual_obstacle_entered(Object* body)
	{
		body->notification(NOTIFICATION_CHAR_VISUAL_ENTERED);
	}
	void visual_obstacle_exited(Object* body)
	{
		body->notification(NOTIFICATION_CHAR_VISUAL_EXITED);
	}
};

#endif