#include "EnttEditor/MetaReflect.h"

#include <string>

#include "EnttEditor/Header/MetaContext.h"

#include "EnttEditor/MetaReflectImplement.h"



using namespace EnttEditor;

IMPLEMENT_META_INIT(void)
{
    META_TYPE(int);
    META_TYPE(unsigned int);

    META_TYPE(float);
    META_TYPE(bool);

    META_TYPE(std::string);

    META_TYPE(entt::entity);
}
FINISH_REFLECT();