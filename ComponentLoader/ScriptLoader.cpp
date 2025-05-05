
#include "pch.h"
#include "ScriptLoader.h"
#include <rttr/registration>
#include <iostream>

#include "test123.h"


void ScriptInitialise() {
    Registry& registry = Registry::instance();
    componentRegistry& ComponentRegistry = componentRegistry::instance();

        ComponentRegistry.registerComponent<test123>(
 rttr::type::get<test123>().get_name().to_string(),
        [&registry](EntityRegistry::Entity entity) {
            registry.addComponent<test123>(entity, {});
});
}
void ScriptDestroy()
{
    componentRegistry& ComponentRegistry = componentRegistry::instance();
ComponentRegistry.unregisterComponent(rttr::type::get<test123>().get_name().to_string());
 }
RTTR_PLUGIN_REGISTRATION
{

rttr::registration::class_<test123>("test123")
		.constructor<>()
		.method("OnStart", &test123::OnStart)
		.method("OnUpdate", &test123::OnUpdate)
     .property("speed", &test123::speed)
;
}