#include "ExampleEntityWindow.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_stdlib.h>

#include <entt/entt.hpp>
#include <glm/gtx/quaternion.hpp>

#include "EnttEditor/Header/Inspector.h"
#include "EnttEditor/MetaReflect.h"
#include "EnttEditor/MetaReflectImplement.h"

using namespace DemoWindow;

TransformComponent::TransformComponent() { m_Rotation = glm::identity<glm::quat>(); }

TransformComponent::TransformComponent(const glm::vec3& pos, const glm::quat& rot, const glm::vec3& scl)
    : m_Position(pos), m_Rotation(rot), m_Scale(scl)
{
}

TransformComponent::TransformComponent(const glm::mat4& matrix)
{
    m_Position = glm::vec3(matrix[3]);

    m_Rotation = glm::toQuat(matrix);

    m_Scale =
        glm::vec3(glm::length(glm::vec3(matrix[0])), glm::length(glm::vec3(matrix[1])), glm::length(glm::vec3(matrix[2])));
}

IMPLEMENT_REFLECT_COMPONENT(NameComponent)
{
    meta.func<&entt::registry::emplace<NameComponent, const std::string&>>(f_AddComponent);
    meta.prop(p_Internal);
    meta.ctor<const std::string&>();

    meta.data<&NameComponent::name>("NAME"_hs) PROP_DISPLAYNAME("Name");
}
FINISH_REFLECT()

IMPLEMENT_REFLECT_COMPONENT(TransformComponent)
{
    meta.data<&TransformComponent::m_Position>("M_Pos"_hs) PROP_DISPLAYNAME("Position");

    meta.data<&TransformComponent::m_Rotation>("M_Rot"_hs) PROP_DISPLAYNAME("Rotation");

    meta.data<&TransformComponent::m_Scale>("M_Scale"_hs) PROP_DISPLAYNAME("Scale");
}
FINISH_REFLECT();

DemoEntityWindow::DemoEntityWindow(entt::registry* reg) : m_Reg(reg) {}

bool containsIgnoreCase(const std::string& str, const std::string& sub)
{
    if (sub.empty()) return true;
    // Convert both strings to lowercase
    std::string strLower = str;
    std::string subLower = sub;
    std::transform(strLower.begin(), strLower.end(), strLower.begin(), [](unsigned char c) { return std::tolower(c); });
    std::transform(subLower.begin(), subLower.end(), subLower.begin(), [](unsigned char c) { return std::tolower(c); });

    // Check if strLower contains subLower
    return strLower.find(subLower) != std::string::npos;
}

void DemoEntityWindow::DrawComponentFilter(std::vector<entt::meta_type>& currentData, std::string& ComponentSearch)
{
    if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0)) ImGui::SetKeyboardFocusHere(0);

    ImGui::InputText("SearchName", &ComponentSearch);

    for (auto&& [metaID, metaType] : entt::resolve())
    {
        if (!metaType.prop(T_ReflectedComponent)) continue;

        auto name = metaType.prop(p_DisplayName).value().cast<std::string>();
        if (!containsIgnoreCase(name, ComponentSearch))
        {
            continue;
        }

#ifndef ShowInternal
        if (metaType.prop(p_Internal)) continue;

#endif
        entt::meta_type metaTypeCopy = metaType;

        auto selectedIndex =
            std::find_if(std::begin(currentData), std::end(currentData),
                         [metaTypeCopy](entt::meta_type& type)
                         { return static_cast<unsigned>(type.id()) == static_cast<unsigned>(metaTypeCopy.id()); });
        bool selected = selectedIndex != std::end(currentData);

        ImGui::PushID(metaID);

        if (ImGui::Selectable(name.c_str(), selected))
        {
            if (selected)
                currentData.erase(selectedIndex);
            else
                currentData.emplace_back(metaType);
        }
        ImGui::PopID();
    }
}

void DemoEntityWindow::Draw(bool& show)
{
    if (m_SelectedEntity.m_Entity != entt::null)
    {
        if (!m_Reg->valid(m_SelectedEntity.m_Entity)) SetSelectedEntity(entt::null);
    }

    if (ImGui::Begin("Entities", &show, ImGuiWindowFlags_NoScrollbar))
    {
        static ImVec2 entityList{0, 100};
        static ImVec2 componentList{0, 500};

        {
            static std::vector<entt::meta_type> componentFilter{};

            std::string filterTitle = "ComponentFilter";
            if (!componentFilter.empty()) filterTitle += "(Active)";

            if (ImGui::BeginPopup("FilterComponentModal"))
            {
                std::string lll = "";
                DrawComponentFilter(componentFilter, lll);
                ImGui::EndPopup();
            }

            ImGui::PushItemWidth(-1);
            ImGui::ListBoxHeader("##Empty", ImVec2(0, entityList.y));

            for (auto [entity] : m_Reg->storage<entt::entity>().each())
            {
                std::string searchFilter = "";
                DrawEnttFilterd(entity, searchFilter, componentFilter);
            }
            ImGui::ListBoxFooter();
        }

        ImVec2 realCursor = AddImVec(ImGui::GetCursorPos(), ImVec2(0, -ImGui::GetScrollY()));

        ImVec2 max = AddImVec(AddImVec(ImGui::GetWindowPos(), ImVec2(ImGui::GetWindowWidth(), 5)), realCursor);

        ImGui::SplitterBehavior(ImRect(AddImVec(realCursor, ImGui::GetWindowPos()), max), ImGui::GetID("Entities"), ImGuiAxis_Y,
                                &entityList.y, &componentList.y, 50, 50, 5, 1);
        ImGui::Dummy(ImVec2(0, 5));

        ImGui::BeginChild("entityList");
        {
            if (m_Reg->valid(m_SelectedEntity.m_Entity))
            {
                if (ImGui::Button("AddComponent"))
                {
                    ComponentSearch = "";
                    ImGui::OpenPopup("AddComponentModal");
                }

                // For each Storage that contains current entity

                for (auto&& [id, storage] : m_Reg->storage())
                {
                    if (storage.contains(m_SelectedEntity.m_Entity))
                    {
                        auto componentMeta = entt::resolve(storage.type());
                        
                        // Entity has no meta data...
                        if (!componentMeta) continue;

                        if (!componentMeta.prop(T_ReflectedComponent))
                        {
                            if (componentMeta.prop(p_Internal)) return;  // This combo will probably never happen...

                            if (ImGui::CollapsingHeader(componentMeta.info().name().data(),
                                                        ImGuiTreeNodeFlags_CollapsingHeader))
                            {
                                ImGui::TextColored(ImColor(255, 0, 0),
                                                   "Component is not reflected, Add 'REFLECT_COMPONENT' macro");
                            }

                            continue;
                        }

                        // Call Inspector::Inspect trough meta..
                        entt::meta_any metaProp = componentMeta.from_void(storage.value(m_SelectedEntity.m_Entity));
                        EnttEditor::Inspector::InspectComponent(*m_Reg, m_SelectedEntity.m_Entity, metaProp);
                    }
                }

                if (ImGui::BeginPopup("AddComponentModal"))
                {
                    if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0))
                        ImGui::SetKeyboardFocusHere(0);

                    ImGui::InputText("SearchName", &ComponentSearch);

                    for (auto&& [metaID, metaType] : entt::resolve())
                    {
                        if (!metaType.prop(T_ReflectedComponent)) continue;

#ifndef ShowInternal
                        if (metaType.prop(p_Internal)) continue;

#endif
                        std::string componentName = metaType.prop(p_DisplayName).value().cast<std::string>();
                        if (!containsIgnoreCase(componentName, ComponentSearch)) continue;

                        if (std::find(std::begin(m_SelectedEntity.m_Components), std::end(m_SelectedEntity.m_Components),
                                      metaType.id()) != m_SelectedEntity.m_Components.end())
                            continue;

                        ImGui::PushID(metaID);

                        auto flags = ImGuiSelectableFlags_None;

                        auto f = metaType.func(f_AddComponent);

                        if (!f) flags = ImGuiSelectableFlags_Disabled;

                        if (ImGui::Selectable(componentName.c_str(), false, flags))
                        {
                            // TODO fix calling it with parameters
                            // We do this by creating a modal popup, and showing args from function (YES THIS WORKS :O)
                            // f.arg()
                            f.invoke({}, entt::forward_as_meta(*m_Reg), m_SelectedEntity.m_Entity);
                            // Refresh active selection
                            SetSelectedEntity(m_SelectedEntity.m_Entity);
                        }
                        ImGui::PopID();
                    }
                    ImGui::EndPopup();
                }
            }
        }
        ImGui::EndChild();
    }
    ImGui::End();
};

void DemoEntityWindow::SetSelectedEntity(entt::entity e)
{
    // This function caches the components a entity has allowing us to iterate faster over type of object.
    m_SelectedEntity = {};
    m_SelectedEntity.m_Entity = e;

    if (m_Reg->valid(e))
        for (auto&& [id, storage] : m_Reg->storage())
        {
            if (storage.contains(e))
            {
                m_SelectedEntity.m_Components.emplace_back(entt::resolve(storage.type()).id());
            }
        }
}

bool MatchComponentFilter(entt::registry& reg, entt::entity e, const std::vector<entt::meta_type>& components)
{
    if (components.empty()) return true;

    for (auto componentType : components)
    {
        auto l = componentType.func(f_ContainsComponent).invoke({}, entt::forward_as_meta(reg), e);
        if (!l.cast<bool>()) return false;
    }
    return true;
}

void DemoEntityWindow::DrawEnttFilterd(entt::entity e, const std::string& nameFilter,
                                       const std::vector<entt::meta_type>& components)
{
    std::string label = m_Reg->get_or_emplace<NameComponent>(e).name + " (" + std::to_string(static_cast<unsigned>(e)) + ")";

    if (!(containsIgnoreCase(label, nameFilter) && MatchComponentFilter(*m_Reg, e, components))) return;

    {
        ImGui::PushID(static_cast<int>(e));
        ImGuiTreeNodeFlags flags =
            ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_Leaf;

        if (m_SelectedEntity.m_Entity == e) flags += ImGuiTreeNodeFlags_Selected;

        bool isOpen = ImGui::TreeNodeEx((label).c_str(), flags);
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
        {
            ImGui::SetDragDropPayload("EntiyID", &e,
                                      sizeof(entt::entity));  // Set payload to carry the index of our item (could be anything)
            ImGui::Text("%s", label.c_str());
            ImGui::EndDragDropSource();
        }

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("EntiyID"))
            {
                IM_ASSERT(payload->DataSize == sizeof(entt::entity));

                entt::entity payload_n = *(const entt::entity*)payload->Data;
                // TODO(PUBLIC_RELEASE) give people hook to this...
                // ::Transform::SetParent(e, payload_n);
            }
            ImGui::EndDragDropTarget();
        }

        if (ImGui::IsItemClicked() && !ImGui::IsItemEdited() && !ImGui::IsMouseDragging(0, 0.1f))
            if (e != m_SelectedEntity.m_Entity) SetSelectedEntity(e);

        if (isOpen)
        {
            ImGui::TreePop();
        }

        ImGui::PopID();
    }

    return;
}
