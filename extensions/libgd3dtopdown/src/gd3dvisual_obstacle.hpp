#ifdef WIN32
#include <windows.h>
#endif
#ifndef GD3DVISUAL_OBSTACLE
#define GD3DVISUAL_OBSTACLE

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/global_constants.hpp>

#include <godot_cpp/classes/static_body3d.hpp>


using namespace godot;

class GD3Dvisual_obstacle : public StaticBody3D
{
	GDCLASS(GD3Dvisual_obstacle, StaticBody3D)

	GD3Dvisual_obstacle();
	~GD3Dvisual_obstacle();

protected:
	static void _bind_methods();
private:
	bool auto_invisible = false;
public:

	virtual void on_enter_obstacle();
	virtual void on_exit_obstacle();

	bool get_auto_invisible() const;
	void set_auto_invisible(const bool var);
};

#endif 

