#pragma once
#include <entt/entt.hpp>
#include <string>

using entt::literals::operator""_hs;
namespace EnttEditor
{
class Inspector
{
public:
    static void InspectComponent(entt::registry& reg, entt::entity owner, entt::meta_any& data);

    // This inspection checks any type of containers, Objects,Arrays ect
    static void InspectClass(entt::registry& reg, entt::meta_any& elementObject, const unsigned elementID, bool createTreeNode = true);

    /// <summary>
    ///
    /// </summary>
    /// <param name="propertyMeta"></param>
    /// <param name="property"></param>
    /// <param name="propertyID">The id of the property in the parrent class</param>
    static void InspectProperty(entt::registry& reg, entt::meta_any& property, entt::meta_data& propertyMeta, const unsigned propertyID);

    /// <summary>
    /// This is the beating heart, It converts the generic objectInformation in something thats useful(But templated)
    /// For users to implement the ElementInspection
    /// </summary>
    /// <typeparam name="Type"></typeparam>
    /// <param name="name"></param>
    /// <param name="value"></param>
    /// <param name="meta"></param>
    /// We Should make this function not templated, only the inner function we call
    static void InspectType(entt::registry& reg, std::string& name, entt::meta_any& value, entt::meta_data& meta);

    // TODO add support for member functions and variables... and return variables?
    //Inspect function support is only a proof of concept.
    static void InspectFunction(entt::registry& reg, entt::meta_func& func);

private:
    /// <summary>
    /// This function deal with properties before PropertyInspect gets invoked
    /// </summary>
    /// <param name="name"></param>
    /// <param name="value"></param>
    /// <param name="meta"></param>
    static void PreInspect(const std::string& name, entt::meta_any& value, const entt::meta_data& meta);

    /// <summary>
    /// This function deal with properties after PropertyInspect got invoked
    /// </summary>
    /// <param name="name"></param>
    /// <param name="value"></param>
    /// <param name="meta"></param>
    static void PostInspect(const std::string& name, entt::meta_any& value, const entt::meta_data& meta);
};
}  // namespace EnttEditor