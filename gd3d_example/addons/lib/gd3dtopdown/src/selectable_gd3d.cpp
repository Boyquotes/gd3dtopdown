#include "selectable_gd3d.hpp"

void SelectableGD3D::_bind_methods()
{
    ADD_SIGNAL(MethodInfo("selected_signal"));
    ADD_SIGNAL(MethodInfo("unselected_signal"));
    ADD_SIGNAL(MethodInfo("selected_signal_info", PropertyInfo(Variant::RID, "object_rid"), PropertyInfo(Variant::OBJECT, "object")));
    ADD_SIGNAL(MethodInfo("unselected_signal_info", PropertyInfo(Variant::RID, "object_rid"), PropertyInfo(Variant::OBJECT, "object")));
}
void SelectableGD3D::_notification(int p_what)
{
    switch (p_what)
    {
        case NOTIFICATION_GD3D_AIMED:
        {
            on_selected();
        }break;
        case NOTIFICATION_GD3D_UNAIMED:
        {
            on_unselected();
        }break;
    }
}
void SelectableGD3D::on_selected()
{
    selected = true;
    emit_signal("selected_signal");
    emit_signal("selected_signal_info", get_rid(), this);
}
void SelectableGD3D::on_unselected()
{
    selected = false;
    emit_signal("unselected_signal");
    emit_signal("unselected_signal_info", get_rid(), this);
}
bool SelectableGD3D::is_selected()
{
    return selected;
}