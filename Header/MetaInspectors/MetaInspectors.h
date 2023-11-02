#pragma once

#include <imgui/imgui.h>

#include <entt/entt.hpp>

#include "EnttEditor/Header/Defines.h"
#include "EnttEditor/MetaReflect.h"

namespace EnttEditor
{

/// <summary>
/// All the function Inspectors...
/// In the ideal world the user can add its own outside this file.
/// </summary>
namespace MetaInspectors
{
#define GetOrDefault(KEY, TYPE, DEFAULT) \
    meta.prop(KEY) && meta.prop(KEY).value() ? meta.prop(KEY).value().cast<TYPE>() : DEFAULT

template <typename Type>
STATICTEMPLATE void MetaInspect(const std::string& name, Type& value, const entt::meta_data& meta)
{
    // We don't know how to draw this type. Contact Damian if you don't know how to deal with this
    ImGui::TextColored(ImColor(255, 0, 0), "Missing MetaInspect for: '%s'", meta.type().info().name().data());
}

template <>
STATICTEMPLATE void MetaInspect<float>(const std::string& name, float& value, const entt::meta_data& meta)
{
    auto speed = GetOrDefault(p_SliderSpeed, float, 1);

    auto min = GetOrDefault(p_ValueMin, float, 0.0f);
    auto max = GetOrDefault(p_ValueMax, float, 0.0f);

    ImGui::Text(name.c_str());
    ImGui::SameLine();
    ImGui::DragFloat(("##" + name).c_str(), &value, speed, min, max);
}

template <>
STATICTEMPLATE void MetaInspect<int>(const std::string& name, int& value, const entt::meta_data& meta)
{
    auto speed = GetOrDefault(p_SliderSpeed, float, 1);

    auto min = GetOrDefault(p_ValueMin, int, 0);
    auto max = GetOrDefault(p_ValueMax, int, 0);

    ImGui::Text(name.c_str());
    ImGui::SameLine();
    ImGui::DragInt(("##" + name).c_str(), &value, speed, min, max);
}

template <>
STATICTEMPLATE void MetaInspect<unsigned int>(const std::string& name, unsigned int& value, const entt::meta_data& meta)
{
    auto speed = GetOrDefault(p_SliderSpeed, float, 1);

    auto min = GetOrDefault(p_ValueMin, int, 0);
    auto max = GetOrDefault(p_ValueMax, int, 0);

    ImGui::Text(name.c_str());
    ImGui::SameLine();
    ImGui::DragScalar(("##" + name).c_str(), ImGuiDataType_U32, &value, speed, &min, &max);
}

template <>
STATICTEMPLATE void MetaInspect<bool>(const std::string& name, bool& value, const entt::meta_data& meta)
{
    ImGui::Text(name.c_str());
    ImGui::SameLine();
    ImGui::Checkbox(("##" + name).c_str(), &value);
}

template <>
STATICTEMPLATE void MetaInspect<std::string>(const std::string& name, std::string& value, const entt::meta_data& meta)
{
    static char* n = new char[500];  // This is not great but required if we don't have the imgui string thing....
                                     // TODO detect if user included that file..
    assert(value.size() <= 500);

    strcpy_s(n, sizeof(char) * 500, value.c_str());

    ImGui::Text(name.c_str());
    ImGui::SameLine();
    if (ImGui::InputText(("##" + name).c_str(), n, 500)) value = n;
}

/// <summary>
/// Function that converts the metaAny to the Actual Type
/// And invokes the MetaInspection
/// </summary>
/// <typeparam name="Type"></typeparam>
/// <param name="name"></param>
/// <param name="value"></param>
/// <param name="meta"></param>
template <typename Type>
static void Inspect(const char* name, entt::meta_any& value, const entt::meta_data& meta)
{
    // We cast the meta_any to the actual type We know the actual type the moment we Registerd Inspect to the meta_type
    Type& typedValue = value.cast<Type&>();
    MetaInspect<Type>(name, typedValue, meta);
}

template <typename Type>
static void InspectEnum(const char* name, entt::meta_any& value, const entt::meta_data& meta)
{
    Type t = value.cast<Type>();

    struct typeID
    {
        unsigned id;
        std::string name;
    };

    // Cacheched array for sizes, just reuse it.
    static std::vector<typeID> enumNames = std::vector<typeID>();  // Lets assume most enums don't need more then 5
    int amount = 0;
    std::string active = "NONE";

    for (auto&& [enumValueID, elementType] : value.type().data())
    {
        std::string name = "UNKOWN";
        if (elementType.prop(p_DisplayName))
        {
            name = elementType.prop(p_DisplayName).value().cast<std::string>();
        }
        if (enumNames.size() <= amount) enumNames.resize(enumNames.size() + 5);

        enumNames[amount] = {enumValueID, name};
        amount++;

        if (elementType.get({}).cast<Type>() == t)
        {
            active = name;
        }
    }

    if (!ImGui::BeginCombo(name, active.c_str())) return;

    for (int i = 0; i < amount; ++i)
    {
        if (ImGui::Selectable(enumNames[i].name.c_str(), enumNames[i].name == active))
        {
            value = value.type().data(enumNames[i].id).get({});

            ImGui::EndCombo();
            return;
        }
    }

    ImGui::EndCombo();
}
}  // namespace MetaInspectors
}  // namespace EnttEditor