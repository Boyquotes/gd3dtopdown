#include "GD3Dvisual_obstacle.hpp"

GD3Dvisual_obstacle::GD3Dvisual_obstacle()
{
}

GD3Dvisual_obstacle::~GD3Dvisual_obstacle()
{
}

void GD3Dvisual_obstacle::_bind_methods()
{
	//Signals
	ADD_SIGNAL(MethodInfo("visual_disappear_signal", PropertyInfo(Variant::RID, "object_rid"), PropertyInfo(Variant::OBJECT, "object")));
	ADD_SIGNAL(MethodInfo("visual_appear_signal", PropertyInfo(Variant::RID, "object_rid"), PropertyInfo(Variant::OBJECT, "object")));
	ClassDB::bind_method(D_METHOD("emit_visual_disappear"), &GD3Dvisual_obstacle::emit_visual_disappear);
	ClassDB::bind_method(D_METHOD("emit_visual_appear"), &GD3Dvisual_obstacle::emit_visual_appear);
}

void GD3Dvisual_obstacle::emit_visual_disappear()
{
	emit_signal("visual_disappear_signal", get_rid(), this);
}
void GD3Dvisual_obstacle::emit_visual_appear()
{
	emit_signal("visual_appear_signal", get_rid(), this);
}
