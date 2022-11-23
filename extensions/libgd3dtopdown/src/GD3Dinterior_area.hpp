#ifndef GD3DINTERIOR_DETECTOR
#define GD3DINTERIOR_DETECTOR
#ifdef WIN32
#include <windows.h>
#endif

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/area3d.hpp>

#include <godot_cpp/classes/physics_body3d.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node3d.hpp>

using namespace godot;

class GD3Dinterior_area : public Area3D
{
	GDCLASS(GD3Dinterior_area, Area3D);

protected:
	static void _bind_methods();
	void _notification(int p_what);
	bool entered;
public:
#include "GD3Dhelpers.hpp"

	bool is_entered();
	bool should_invisible();
};



#endif