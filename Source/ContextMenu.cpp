#include "EnttEditor/Header/ContextMenu.h"

#include <imgui/imgui.h>

#include "EnttEditor/MetaReflect.h"
using namespace EnttEditor;

void ContextMenu::GenerateComponentContext(entt::registry& reg, const entt::entity& owner, entt::meta_any& component)
{
    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::Button("Remove"))
        {
            component.type().func(f_RemoveComponent).invoke({}, entt::forward_as_meta(reg), owner);
        }

        ImGui::EndPopup();
    }

    // Idk if we do anything special here at some point, but we just define right click menu for components here
}
