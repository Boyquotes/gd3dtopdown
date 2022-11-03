#include "GD3Dselectable_node.hpp"
GD3Dselectable_node::GD3Dselectable_node()
{
}

GD3Dselectable_node::~GD3Dselectable_node()
{
}
void GD3Dselectable_node::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("on_selected"), &GD3Dselectable_node::on_selected);
    ClassDB::bind_method(D_METHOD("on_unselected"), &GD3Dselectable_node::on_unselected);

    ADD_SIGNAL(MethodInfo("selected_signal"));
    ADD_SIGNAL(MethodInfo("unselected_signal"));
    ADD_SIGNAL(MethodInfo("selected_signal_info", PropertyInfo(Variant::RID, "object_rid"), PropertyInfo(Variant::OBJECT, "object")));
    ADD_SIGNAL(MethodInfo("unselected_signal_info", PropertyInfo(Variant::RID, "object_rid"), PropertyInfo(Variant::OBJECT, "object")));
}

void GD3Dselectable_node::on_selected()
{
    selected = true;
    emit_signal("selected_signal");
    emit_signal("selected_signal_info", get_rid(), this);
}
void GD3Dselectable_node::on_unselected()
{
    selected = false;
    emit_signal("unselected_signal");
    emit_signal("unselected_signal_info", get_rid(), this);
}
bool GD3Dselectable_node::is_selected()
{
    return selected;
}