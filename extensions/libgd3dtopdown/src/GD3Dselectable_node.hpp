#ifndef GD3DSELECTABLE_NODE
#define GD3DSELECTABLE_NODE
#ifdef WIN32
#include <windows.h>
#endif

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/static_body3d.hpp>


using namespace godot;

class GD3Dselectable_node : public StaticBody3D
{
	GDCLASS(GD3Dselectable_node, StaticBody3D);

protected:
	static void _bind_methods();
	void _notification(int p_what);

	bool selected;
public:
#include "GD3Dhelpers.hpp"

	void on_selected();
	void on_unselected();
	bool is_selected();
};

#endif