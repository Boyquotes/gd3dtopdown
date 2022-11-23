#include "GD3Dvisual_obstacle.hpp"

void GD3Dvisual_obstacle::_bind_methods()
{
	ADD_SIGNAL(MethodInfo("obstacle_entered_signal", PropertyInfo(Variant::OBJECT, "object")));
	ADD_SIGNAL(MethodInfo("obstacle_exited_signal", PropertyInfo(Variant::OBJECT, "object")));
}

void  GD3Dvisual_obstacle::_notification(int p_what)
{
	
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE:
		{
			if(ProjectSettings::get_singleton()->has_setting("gd3d/aim_ignore_mask"))
				ignoremask =(uint32_t)ProjectSettings::get_singleton()->get_setting("gd3d/aim_ignore_mask");
			entered_by_char = false;
			is_invisible = false;
			collision_layer = get_collision_layer();

		} break;
		case NOTIFICATION_IAREA_ENTERED:
		{
			GD3Dinterior_area* p_area = dynamic_cast<GD3Dinterior_area*>(get_parent());
			if (p_area)
			{
				if(p_area->should_invisible())obstacle_entered();
			}
		}break;
		case NOTIFICATION_IAREA_EXITED:
		{
			GD3Dinterior_area* p_area = dynamic_cast<GD3Dinterior_area*>(get_parent());
			if (p_area)
			{
				if (p_area->should_invisible() ||
					entered_by_char) break;
				obstacle_exited();
			}
		}break;
		case NOTIFICATION_CHAR_VISUAL_ENTERED:
		{
			obstacle_entered();
			entered_by_char = true;
		}break;
		case NOTIFICATION_CHAR_VISUAL_EXITED:
		{
			entered_by_char = false;
			GD3Dinterior_area* p_area = dynamic_cast<GD3Dinterior_area*>(get_parent());
			if (p_area && p_area->should_invisible()) return;
			obstacle_exited();
		}break;
	}
}

void GD3Dvisual_obstacle::obstacle_entered()
{
	if (is_invisible) return;
	is_invisible = true;
	set_collision_layer((collision_layer & ignoremask) ^ collision_layer);
	propagate_notification(NOTIFICATION_VOBSTACLE_INVISIBLE);
	emit_signal("obstacle_entered_signal", get_rid(), this);
}
void GD3Dvisual_obstacle::obstacle_exited()
{
	if (!is_invisible) return;
	is_invisible = false;
	set_collision_layer(collision_layer);
	propagate_notification(NOTIFICATION_VOBSTACLE_VISIBLE);
	emit_signal("obstacle_exited_signal", get_rid(), this);
}





