#include "EnttEditor/Header/MetaInspectors/GlmMetaInspectors.h"

#include "EnttEditor/Header/MetaContext.h"
#include "EnttEditor/MetaReflectImplement.h"

IMPLEMENT_META_INIT(glm)
{
    META_TYPE(glm::vec2);
    META_TYPE(glm::vec3);
    META_TYPE(glm::vec4);
    META_TYPE(glm::quat);
}
FINISH_REFLECT()
