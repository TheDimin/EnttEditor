#pragma once
#include <entt/entt.hpp>
#include <string>

#include "EnttEditor/Header/MetaContext.h"
#include "EnttEditor/Include.hpp"
#include "EnttEditor/MetaInspectorIncludes.h"
#include "EnttEditor/MetaReflect.h"

#include "EnttEditor/Header/MetaInspectors/MetaInspectors.h"


using entt::literals::operator""_hs;
namespace EnttEditor
{
template <typename T>
static inline entt::meta_factory<T> ReflectComponent(const std::string& name)
{
    // This is done to make sure Meta is initialized (For dll's)
    entt::locator<entt::meta_ctx>::reset(MetaContext::GetMetaContext());

    auto factoryT = entt::meta<T>();

    // Entt funcs
    // Only enable this constructor if there is a default constructor..Users
    if constexpr (std::is_constructible_v<T>)
    {
        factoryT.template func<&entt::registry::emplace<T>>(f_AddComponent);
        factoryT.template func<&entt::registry::emplace_or_replace<T>>(f_TryAddComponent);
        factoryT.template func<&entt::registry::patch<T>>(f_PatchComponent);
    }
    factoryT.template func<&entt::registry::erase<T>>(f_RemoveComponent);
    factoryT.template func<&entt::registry::any_of<T>>(f_ContainsComponent);

    EnttEditor::MetaTypes::ReflectComponent(name, factoryT);

    // Setup
    factoryT.type(entt::hashed_string{name.c_str()});
    factoryT.prop(T_ReflectedComponent);
    factoryT PROP_DISPLAYNAME(name);

    return factoryT;
}

template <typename T>
static inline entt::meta_factory<T> ReflectObject(const std::string& name)
{
    entt::locator<entt::meta_ctx>::reset(MetaContext::GetMetaContext());

    auto factoryT = entt::meta<T>();

    EnttEditor::MetaTypes::ReflectObject(name, factoryT);

    factoryT.prop(T_ReflectedObject);
    factoryT.type(entt::hashed_string{name.c_str()});
    factoryT PROP_DISPLAYNAME(name);

    return factoryT;
}

template <typename T>
static inline entt::meta_factory<T> ReflectSystem(const std::string& name)
{
    entt::locator<entt::meta_ctx>::reset(MetaContext::GetMetaContext());

    auto factoryT = entt::meta<T>();

    EnttEditor::MetaTypes::ReflectSystem(name, factoryT);

    factoryT.prop(T_ReflectedSystem);
    factoryT.type(entt::hashed_string{name.c_str()});
    factoryT PROP_DISPLAYNAME(name);

    return factoryT;
}

template <typename T>
static inline entt::meta_factory<T> ReflectEnum(const std::string& name)
{
    entt::locator<entt::meta_ctx>::reset(MetaContext::GetMetaContext());

    auto factoryT = entt::meta<T>();

    factoryT.template func<&MetaInspectors::InspectEnum<T>>(f_Inspect);
    EnttEditor::MetaTypes::ReflectEnum(name, factoryT);

    factoryT.prop(T_ReflectedEnum);
    factoryT.type(entt::hashed_string{name.c_str()});
    factoryT PROP_DISPLAYNAME(name);

    return factoryT;
}
template <class Type>
static auto TypeInspect()
{
    auto type = entt::meta<Type>();
    type.template func<&MetaInspectors::Inspect<Type>>(f_Inspect);
    return type;
}

}

#define IMPLEMENT_REFLECT_COMPONENT(TYPE) \
    int TYPE::InitTypeReflect()           \
    {                                     \
        [[maybe_unused]] auto meta = EnttEditor::ReflectComponent<TYPE>((#TYPE));

#define IMPLEMENT_REFLECT_OBJECT(TYPE) \
    int TYPE::InitTypeReflect()        \
    {                                  \
        [[maybe_unused]] auto meta = EnttEditor::ReflectObject<TYPE>((#TYPE));

#define IMPLEMENT_REFLECT_SYSTEM(TYPE) \
    int TYPE::InitTypeReflect()        \
    {                                  \
        [[maybe_unused]] auto meta = EnttEditor::ReflectSystem<TYPE>((#TYPE));

#define IMPLEMENT_REFLECT_ENUM(TYPE)                 \
    int ENUMREFLECTS::TYPE_##TYPE::InitTypeReflect() \
    {                                                \
        [[maybe_unused]] auto meta = EnttEditor::ReflectEnum<TYPE>((#TYPE));

#define FINISH_REFLECT() \
    return -1;           \
    }

#define META_TYPE(Type, ...)                                                  \
    {                                                                         \
        [[maybe_unused]] entt::meta_factory<Type> meta = EnttEditor::TypeInspect<Type>(); \
        __VA_ARGS__                                                           \
    }

#define META_ENUM(Type, ...)                                                                \
    {                                                                                       \
        [[maybe_unused]] entt::meta_factory<Type> meta = EnttEditor::ReflectEnum<Type>((#Type)); \
        __VA_ARGS__                                                                         \
    }

#define IMPLEMENT_META_INIT(TYPE)                     \
    int TYPE_REFLECTS::TYPE_##TYPE::InitTypeReflect() \
    {                                                 \
        entt::locator<entt::meta_ctx>::reset(EnttEditor::MetaContext::GetMetaContext());