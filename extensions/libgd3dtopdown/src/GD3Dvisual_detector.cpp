#include "GD3Dvisual_detector.hpp"

void GD3Dvisual_detector::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("visual_obstacle_entered"), &GD3Dvisual_detector::visual_obstacle_entered);
	ClassDB::bind_method(D_METHOD("visual_obstacle_exited"), &GD3Dvisual_detector::visual_obstacle_exited);
}

void GD3Dvisual_detector::_notification(int p_what)
{
	switch (p_what)
	{
	case NOTIFICATION_ENTER_TREE:
	{
		if (!is_connected("body_entered", Callable(this, "visual_obstacle_entered")))
			connect("body_entered", Callable(this, "visual_obstacle_entered"));
		if (!is_connected("body_exited", Callable(this, "visual_obstacle_exited")))
			connect("body_exited", Callable(this, "visual_obstacle_exited"));
	}break;
	case NOTIFICATION_EXIT_TREE:
	{
		if (is_connected("body_entered", Callable(this, "visual_obstacle_entered")))
			disconnect("body_entered", Callable(this, "visual_obstacle_entered"));
		if (is_connected("body_exited", Callable(this, "visual_obstacle_exited")))
			disconnect("body_exited", Callable(this, "visual_obstacle_exited"));
	}break;
	}
}
void GD3Dvisual_detector::visual_obstacle_entered(Object* body)
{
	body->notification(NOTIFICATION_CHAR_VISUAL_ENTERED);
}
void GD3Dvisual_detector::visual_obstacle_exited(Object* body)
{
	body->notification(NOTIFICATION_CHAR_VISUAL_EXITED);
}