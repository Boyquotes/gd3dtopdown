#ifdef WIN32
#include <windows.h>
#endif
#ifndef GD3DSELECTABLE_NODE
#define GD3DSELECTABLE_NODE

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/static_body3d.hpp>


using namespace godot;

class GD3Dselectable_node : public StaticBody3D
{
	GDCLASS(GD3Dselectable_node, StaticBody3D);
	GD3Dselectable_node();
	~GD3Dselectable_node();

protected:
	static void _bind_methods();
	bool selected;
public:

	void on_selected();
	void on_deselected();
	bool is_selected();
};

#endif