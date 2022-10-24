#ifdef WIN32
#include <windows.h>
#endif
#ifndef GD3DINTERIOR_AREA
#define GD3DINTERIOR_AREA

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
	GD3Dinterior_area();
	~GD3Dinterior_area();

protected:
	static void _bind_methods();
	bool entered;
public:

	void on_enter_area(uint32_t ignoremask);
	void on_exit_area(uint32_t ignoremask);
	bool is_entered();
};



#endif