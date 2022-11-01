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
    ClassDB::bind_method(D_METHOD("on_deselected"), &GD3Dselectable_node::on_deselected);

    ADD_SIGNAL(MethodInfo("selected_signal", PropertyInfo(Variant::RID, "object_rid"), PropertyInfo(Variant::OBJECT, "object")));
    ADD_SIGNAL(MethodInfo("deselected_signal", PropertyInfo(Variant::RID, "object_rid"), PropertyInfo(Variant::OBJECT, "object")));
}

void GD3Dselectable_node::on_selected()
{
    selected = true;
    emit_signal("selected_signal", get_rid(), this);
}
void GD3Dselectable_node::on_deselected()
{
    selected = false;
    emit_signal("deselected_signal", get_rid(), this);
}
bool GD3Dselectable_node::is_selected()
{
    return selected;
}