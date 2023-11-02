#pragma once
#include <entt/entt.hpp>

namespace EnttEditor
{
class MetaContext
{
public:
    static entt::locator<entt::meta_ctx>::node_type GetMetaContext()
    {
        [[maybe_unused]] auto l = entt::locator<entt::meta_ctx>::value_or();

        static auto nodeType = entt::locator<entt::meta_ctx>::handle();

        return nodeType;
    }
};
}  // namespace EnttEditor