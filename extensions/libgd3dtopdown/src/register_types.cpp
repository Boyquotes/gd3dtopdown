#include "register_types.hpp"

#include <godot/gdnative_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "GD3Dtd_character.hpp"
#include "GD3Dvisual_obstacle.hpp"
#include "GD3Dinterior_area.hpp"
#include "GD3Dselectable_node.hpp"
#include "GD3Dinteriors_detector.hpp"
#include "GD3Dvisual_detector.hpp"
#include "GD3Dtween_mesh.h"
using namespace godot;

void initialize_GD3Dtopdown_module(ModuleInitializationLevel p_level)
{
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
    {
		return;
	}
    
    ClassDB::register_class<GD3Dtd_character>();
    ClassDB::register_class<GD3Dvisual_obstacle>();
    ClassDB::register_class<GD3Dinterior_area>();
    ClassDB::register_class<GD3Dselectable_node>();
    ClassDB::register_class<GD3Dinteriors_detector>();
    ClassDB::register_class<GD3Dvisual_detector>();
    ClassDB::register_class<GD3Dtween_mesh>();

}
void uninitialize_GD3Dtopdown_module(ModuleInitializationLevel p_level)
{
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) 
    {
        return;
    }
}

extern "C" {
GDNativeBool GDN_EXPORT GD3Dtopdown_library_init(const GDNativeInterface *p_interface, const GDNativeExtensionClassLibraryPtr p_library, GDNativeInitialization *r_initialization) {
    godot::GDExtensionBinding::InitObject init_obj(p_interface, p_library, r_initialization);

    init_obj.register_initializer(initialize_GD3Dtopdown_module);
    init_obj.register_terminator(uninitialize_GD3Dtopdown_module);

    init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);
    return init_obj.init();
}
}