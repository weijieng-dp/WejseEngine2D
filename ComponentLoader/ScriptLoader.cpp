
#include "pch.h"
#include "ScriptLoader.h"
#include <rttr/registration>
#include <iostream>

#include "../ComponentLoader/Scripts/asdfasdfasdf.h"
#include "../ComponentLoader/Scripts/beuh3.h"
#include "../ComponentLoader/Scripts/bruh.h"
#include "../ComponentLoader/Scripts/bruh2.h"


void ScriptInitialise() {
    Registry& registry = Registry::instance();
    componentRegistry& ComponentRegistry = componentRegistry::instance();

        ComponentRegistry.registerComponent<asdfasdfasdf>(
 rttr::type::get<asdfasdfasdf>().get_name().to_string(),
        [&registry](EntityRegistry::Entity entity) {
            registry.addComponent<asdfasdfasdf>(entity, {});
});
ComponentRegistry.registerComponent<beuh3>(
 rttr::type::get<beuh3>().get_name().to_string(),
        [&registry](EntityRegistry::Entity entity) {
            registry.addComponent<beuh3>(entity, {});
});
ComponentRegistry.registerComponent<bruh>(
 rttr::type::get<bruh>().get_name().to_string(),
        [&registry](EntityRegistry::Entity entity) {
            registry.addComponent<bruh>(entity, {});
});
ComponentRegistry.registerComponent<bruh2>(
 rttr::type::get<bruh2>().get_name().to_string(),
        [&registry](EntityRegistry::Entity entity) {
            registry.addComponent<bruh2>(entity, {});
});
}
void ScriptDestroy()
{
    componentRegistry& ComponentRegistry = componentRegistry::instance();
ComponentRegistry.unregisterComponent(rttr::type::get<asdfasdfasdf>().get_name().to_string());
ComponentRegistry.unregisterComponent(rttr::type::get<beuh3>().get_name().to_string());
ComponentRegistry.unregisterComponent(rttr::type::get<bruh>().get_name().to_string());
ComponentRegistry.unregisterComponent(rttr::type::get<bruh2>().get_name().to_string());
 }
RTTR_PLUGIN_REGISTRATION
{

rttr::registration::class_<asdfasdfasdf>("asdfasdfasdf")
		.constructor<>()
		.method("OnStart", &asdfasdfasdf::OnStart)
		.method("OnUpdate", &asdfasdfasdf::OnUpdate)
     .property("demo", &asdfasdfasdf::demo)
     .property("demo2", &asdfasdfasdf::demo2)
;
rttr::registration::class_<beuh3>("beuh3")
		.constructor<>()
		.method("OnStart", &beuh3::OnStart)
		.method("OnUpdate", &beuh3::OnUpdate)
     .property("demo", &beuh3::demo)
;
rttr::registration::class_<bruh>("bruh")
		.constructor<>()
		.method("OnStart", &bruh::OnStart)
		.method("OnUpdate", &bruh::OnUpdate)
     .property("bruh2", &bruh::bruh2)
     .property("speed", &bruh::speed)
;
rttr::registration::class_<bruh2>("bruh2")
		.constructor<>()
		.method("OnStart", &bruh2::OnStart)
		.method("OnUpdate", &bruh2::OnUpdate)
     .property("bruh", &bruh2::bruh)
     .property("speed", &bruh2::speed)
;
}