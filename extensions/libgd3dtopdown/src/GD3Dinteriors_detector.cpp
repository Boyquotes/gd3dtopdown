#include "GD3Dinteriors_detector.hpp"

void GD3Dinteriors_detector::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("interior_entered"), &GD3Dinteriors_detector::interior_entered);
	ClassDB::bind_method(D_METHOD("interior_exited"), &GD3Dinteriors_detector::interior_exited);
}

void GD3Dinteriors_detector::_notification(int p_what)
{
	switch (p_what)
	{
	case NOTIFICATION_ENTER_TREE:
	{
		if (!is_connected("area_entered", Callable(this, "interior_entered")))
			connect("area_entered", Callable(this, "interior_entered"));
		if (!is_connected("area_exited", Callable(this, "interior_exited")))
			connect("area_exited", Callable(this, "interior_exited"));
	}break;
	case NOTIFICATION_EXIT_TREE:
	{
		if (is_connected("area_entered", Callable(this, "interior_entered")))
			disconnect("area_entered", Callable(this, "interior_entered"));
		if (is_connected("area_exited", Callable(this, "interior_exited")))
			disconnect("area_exited", Callable(this, "interior_exited"));
	}break;
	}
}

void GD3Dinteriors_detector::interior_entered(Object* area)
{
	area->notification(NOTIFICATION_CHAR_AREA_ENTERED);
}
void GD3Dinteriors_detector::interior_exited(Object* area)
{
	area->notification(NOTIFICATION_CHAR_AREA_EXITED);
}