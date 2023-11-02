#pragma once
#include <imgui/imgui.h>

#include <entt/fwd.hpp>
#include <glm/detail/type_quat.hpp>
#include <glm/glm.hpp>

#include "EnttEditor/MetaReflect.h"

namespace DemoWindow
{
class NameComponent
{
public:
    NameComponent() = default;
    NameComponent(const std::string& newName) : name(newName){};

    std::string name = "Entity";
    REFLECT()
};

class TransformComponent
{
public:
    TransformComponent();
    TransformComponent(const glm::vec3& pos, const glm::quat& rot, const glm::vec3& scl);
    TransformComponent(const glm::mat4& mat);

private:
    glm::vec3 m_Position{0.0f, 0.0f, 0.f};
    glm::quat m_Rotation;
    glm::vec3 m_Scale{1.0f, 1.0f, 1.0f};

    mutable glm::mat4 m_Local = glm::identity<glm::mat4>();
    mutable glm::mat4 m_Global = glm::identity<glm::mat4>();
    mutable bool m_Dirty = true;

    REFLECT()
};

// This class doesn't show how to draw Hierarchies (Wiki example will follow)
// But gives general idea of how to deal with the api of drawing objects
class DemoEntityWindow
{
public:
    DemoEntityWindow(entt::registry* reg);

    void Draw(bool& show);

    void SetSelectedEntity(entt::entity e);
    void DrawEnttFilterd(entt::entity e, const std::string& nameFilter, const std::vector<entt::meta_type>& components);

private:
    void DrawComponentFilter(std::vector<entt::meta_type>& currentData, std::string& ComponentSearch);

    inline ImVec2 AddImVec(ImVec2 a, ImVec2 b) { return {a.x + b.x, a.y + b.y}; }

    entt::registry* m_Reg = nullptr;
    struct SelectedEntityInfo
    {
        entt::entity m_Entity = entt::tombstone;
        // List of all the components this entity has
        std::vector<unsigned int> m_Components{};

    } m_SelectedEntity;

    std::string ComponentSearch = "";
};
}  // namespace DemoWindow
