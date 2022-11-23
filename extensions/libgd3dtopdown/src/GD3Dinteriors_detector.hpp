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

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
#include"GD3Dhelpers.hpp"
public:
	void interior_entered(Object* area);
	
	void interior_exited(Object* area);
	
};

#endif