#pragma once
#include <EnttEditor/Header/MetaInspectors/GlmMetaInspectors.h>


#include <entt/entt.hpp>

namespace EnttEditor
{

/////////////////////////////
//	REFLECT INEJCTIONS
// This is the correct place to extend type reflection (Allows you to inject for instance serialization for the type).
/////////////////////////////
///

namespace MetaTypes
{
template <typename T>
static inline void ReflectComponent(const std::string& TypeName, entt::meta_factory<T>& factory)
{
}

template <typename T>
static inline void ReflectObject(const std::string& TypeName, entt::meta_factory<T>& factory)
{
}

template <typename T>
static inline void ReflectSystem(const std::string& TypeName, entt::meta_factory<T>& factory)
{
    // factory.template base<::System>();
}

template <typename T>
static inline void ReflectEnum(const std::string& TypeName, entt::meta_factory<T>& factory)
{
    // factory.template func <&::Serializers::Serialize<T>>(f_SaveType);
    // factory.template func <&::Serializers::Deserialize<T>>(f_LoadType);
}
}  // namespace MetaTypes
}  // namespace EnttEditor
