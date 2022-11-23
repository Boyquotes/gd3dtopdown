#include "interior_area_gd3d.hpp"

void InteriorAreaGD3D::_bind_methods(){}
void InteriorAreaGD3D::_notification(int p_what){
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

bool InteriorAreaGD3D::is_entered()
{
    return entered;
}
bool InteriorAreaGD3D::should_invisible()
{
    InteriorAreaGD3D* p_area = dynamic_cast<InteriorAreaGD3D*>(get_parent());
    if (p_area)
    {
        if(p_area->should_invisible()) return true;
    }
    return entered;
}

