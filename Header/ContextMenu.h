#pragma once
#include <entt/entt.hpp>
#include <string>

namespace EnttEditor
{
class ContextMenu
{
public:
    static void GenerateComponentContext(entt::registry& reg, const entt::entity& owner, entt::meta_any& component);

    static void GenerateEntityContext(const entt::entity&){};

private:
    static inline std::string relPath = "";
};
}  // namespace EnttEditor