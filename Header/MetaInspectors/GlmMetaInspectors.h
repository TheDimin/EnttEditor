#pragma once
#include <glm/gtc/quaternion.hpp>
#include <glm/vec4.hpp>

#include "MetaInspectors.h"

META_INIT(glm)

// TODO damian , way to detect if glm is included
namespace EnttEditor
{
namespace MetaInspectors
{
template <>
STATICTEMPLATE void MetaInspect<glm::vec4>(const std::string& name, glm::vec4& value, const entt::meta_data& meta)
{
    if (meta.prop(p_DrawAsColor))
    {
        ImGui::Text(name.c_str());
        ImGui::SameLine();

        if (meta.prop(p_ReadOnlyInInspector))

            ImGui::ColorEdit4("", &value[0], ImGuiColorEditFlags_NoInputs);
        else
            ImGui::ColorEdit4("", &value[0]);
    }
    else
    {
        auto speed = GetOrDefault(p_SliderSpeed, float, 1);

        auto min = GetOrDefault(p_ValueMin, float, 0);
        auto max = GetOrDefault(p_ValueMax, float, 0);

        ImGui::Text(name.c_str());
        ImGui::SameLine();

        ImGui::DragFloat4("", &value[0], speed, min, max);
    }
}

template <>
STATICTEMPLATE void MetaInspect<glm::vec3>(const std::string& name, glm::vec3& value, const entt::meta_data& meta)
{
    ImGui::Text(name.c_str());
    ImGui::SameLine();

    if (meta.prop(p_DrawAsColor))
    {
        if (meta.prop(p_ReadOnlyInInspector))

            ImGui::ColorEdit3(("##" + name).c_str(), &value[0], ImGuiColorEditFlags_NoInputs);
        else
            ImGui::ColorEdit3(("##" + name).c_str(), &value[0]);
    }
    else
    {
        auto speed = GetOrDefault(p_SliderSpeed, float, 1);

        auto min = GetOrDefault(p_ValueMin, float, 0);
        auto max = GetOrDefault(p_ValueMax, float, 0);

        ImGui::DragFloat3(("##" + name).c_str(), &value[0], speed, min, max);

        if (meta.prop(p_Normalized))
        {
            if (glm::length(value) > 0.0001f) value = glm::normalize(value);
        }
    }
}

template <>
STATICTEMPLATE void MetaInspect<glm::vec2>(const std::string& name, glm::vec2& value, const entt::meta_data& meta)
{
    ImGui::Text(name.c_str());
    ImGui::SameLine();

    if (meta.prop(p_DrawAsColor))
    {
        if (meta.prop(p_ReadOnlyInInspector))

            ImGui::ColorEdit3(("##" + name).c_str(), &value[0], ImGuiColorEditFlags_NoInputs);
        else
            ImGui::ColorEdit3(("##" + name).c_str(), &value[0]);
    }
    else
    {
        auto speed = GetOrDefault(p_SliderSpeed, float, 1);

        auto min = GetOrDefault(p_ValueMin, float, 0);
        auto max = GetOrDefault(p_ValueMax, float, 0);

        ImGui::DragFloat2(("##" + name).c_str(), &value[0], speed, min, max);
    }
}

template <>
STATICTEMPLATE void MetaInspect<glm::quat>(const std::string& name, glm::quat& value, const entt::meta_data& meta)
{
    glm::vec3 eulerRotation = glm::degrees(glm::eulerAngles(value));

    auto speed = GetOrDefault(p_SliderSpeed, float, 1);

    ImGui::Text(name.c_str());
    ImGui::SameLine();
    if (ImGui::DragFloat3(("##" + name).c_str(), (float*)&eulerRotation.x, speed, -180.f, 180.f))
    {
        eulerRotation.y = glm::clamp(eulerRotation.y, -89.9f, 89.9f);
        value = glm::quat(glm::radians(eulerRotation));
    }
}
}  // namespace MetaInspectors
}  // namespace EnttEditor