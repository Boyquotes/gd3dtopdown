#ifndef GD3DINTERIOR_AREA
#define GD3DINTERIOR_AREA
#ifdef WIN32
#include <windows.h>
#endif

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/area3d.hpp>

using namespace godot;
class InteriorDetectorGD3D : public Area3D
{
	GDCLASS(InteriorDetectorGD3D, Area3D);

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
#include "helpers_gd3d.hpp"
public:
	void interior_entered(Object* area);
	
	void interior_exited(Object* area);
	
};

#endif