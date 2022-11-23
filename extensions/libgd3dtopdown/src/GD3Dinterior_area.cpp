#include "GD3Dinterior_area.hpp"

void GD3Dinterior_area::_bind_methods(){}
void GD3Dinterior_area::_notification(int p_what){
    switch (p_what) {
        case NOTIFICATION_ENTER_TREE:
        {
            entered = false;
        } break;
        case NOTIFICATION_CHAR_AREA_ENTERED:
        {
            entered = true;
            propagate_notification(NOTIFICATION_IAREA_ENTERED);
        }break;
        case NOTIFICATION_CHAR_AREA_EXITED:
        {
            entered = false;
            propagate_notification(NOTIFICATION_IAREA_EXITED);
        }break;
    }
}

bool GD3Dinterior_area::is_entered()
{
    return entered;
}
bool GD3Dinterior_area::should_invisible()
{
    GD3Dinterior_area* p_area = dynamic_cast<GD3Dinterior_area*>(get_parent());
    if (p_area)
    {
        if(p_area->should_invisible()) return true;
    }
    return entered;
}

