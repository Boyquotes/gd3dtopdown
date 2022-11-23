#ifndef GD3DINTERIOR_AREA
#define GD3DINTERIOR_AREA
#ifdef WIN32
#include <windows.h>
#endif

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/area3d.hpp>

using namespace godot;
class GD3Dinteriors_detector : public Area3D
{
	GDCLASS(GD3Dinteriors_detector, Area3D);

public:
#include"GD3Dhelpers.hpp"
protected:
	void _notification(int p_what)
	{
		switch (p_what)
		{
			case NOTIFICATION_ENTER_TREE:
			{
				if (!is_connected("area_entered", Callable(this, "interior_entered")))
					connect("area_entered", Callable(this, "interior_entered"));
				if (!is_connected("area_exited", Callable(this, "interior_exited")))
					connect("area_exited", Callable(this, "interior_exited"));
			}break;
			case NOTIFICATION_EXIT_TREE:
			{
				if (is_connected("area_entered", Callable(this, "interior_entered")))
					disconnect("area_entered", Callable(this, "interior_entered"));
				if (is_connected("area_exited", Callable(this, "interior_exited")))
					disconnect("area_exited", Callable(this, "interior_exited"));
			}break;
		}
	}
	static void _bind_methods()
	{
		ClassDB::bind_method(D_METHOD("interior_entered"), &GD3Dinteriors_detector::interior_entered);
		ClassDB::bind_method(D_METHOD("interior_exited"), &GD3Dinteriors_detector::interior_exited);
	}

	void interior_entered(Object* area)
	{
		area->notification(NOTIFICATION_CHAR_AREA_ENTERED);
	}
	void interior_exited(Object* area)
	{
		area->notification(NOTIFICATION_CHAR_AREA_EXITED);
	}
};

#endif