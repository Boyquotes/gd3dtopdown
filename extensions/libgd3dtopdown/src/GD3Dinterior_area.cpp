#include "GD3Dinterior_area.hpp"

GD3Dinterior_area::GD3Dinterior_area() { entered = false; }

GD3Dinterior_area::~GD3Dinterior_area() {}

void GD3Dinterior_area::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("on_enter_area"), &GD3Dinterior_area::on_enter_area);
    ClassDB::bind_method(D_METHOD("on_exit_area"), &GD3Dinterior_area::on_exit_area);

    ADD_SIGNAL(MethodInfo("entered_signal", PropertyInfo(Variant::RID, "object_rid"), PropertyInfo(Variant::OBJECT, "object"), PropertyInfo(Variant::INT, "ignoremask")));
    ADD_SIGNAL(MethodInfo("exited_signal", PropertyInfo(Variant::RID, "object_rid"), PropertyInfo(Variant::OBJECT, "object"), PropertyInfo(Variant::INT, "ignoremask")));
    ADD_SIGNAL(MethodInfo("entered_signal_mask", PropertyInfo(Variant::INT, "ignoremask")));
    ADD_SIGNAL(MethodInfo("exited_signal_mask", PropertyInfo(Variant::INT, "ignoremask")));
}

void GD3Dinterior_area::on_enter_area(uint32_t ignoremask)
{
    entered = true;
    emit_signal("entered_signal", get_rid(), this, ignoremask);
    emit_signal("entered_signal_mask", ignoremask);
}
void GD3Dinterior_area::on_exit_area(uint32_t ignoremask)
{
    entered = false;
    emit_signal("exited_signal", get_rid(), this, ignoremask);
    emit_signal("exited_signal_mask", ignoremask);
}
bool GD3Dinterior_area::is_entered()
{
    return entered;
}

