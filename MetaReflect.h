#pragma once
#include <entt/entt.hpp>

// When this is defined we ignore p_Internal
// #define ShowInternal

using entt::literals::operator""_hs;

constexpr entt::hashed_string T_ReflectedObject = "ReflectedObject"_hs;
constexpr entt::hashed_string T_ReflectedComponent = "ReflectedComponent"_hs;
constexpr entt::hashed_string T_ReflectedSystem = "ReflectedSystem"_hs;
constexpr entt::hashed_string T_ReflectedEnum = "ReflectedEnum"_hs;

constexpr entt::hashed_string p_DisplayName = "DisplayName"_hs;

// This metaFunction is used to draw element types (int,float,vec3..ect)
// When used on classes its considerd that user handles the method of drawing this class
constexpr entt::hashed_string f_Inspect = "Inspect"_hs;

constexpr entt::hashed_string f_AddComponent = "AddComponent"_hs;
constexpr entt::hashed_string f_TryAddComponent = "TryAddComponent"_hs;
constexpr entt::hashed_string f_PatchComponent = "PatchComponentn"_hs;
constexpr entt::hashed_string f_RemoveComponent = "RemoveComponent"_hs;
constexpr entt::hashed_string f_ContainsComponent = "ContainsComponent"_hs;

// PropertyDefines
constexpr entt::hashed_string p_HideFromInspector = "HideFromInspector"_hs;
constexpr entt::hashed_string p_ToolTip = "ToolTip"_hs;
constexpr entt::hashed_string p_ReadOnlyInInspector = "ReadOnly"_hs;
constexpr entt::hashed_string p_Normalized = "Normalized"_hs;
// For glm::Vec3/4 draw it as color picker
constexpr entt::hashed_string p_DrawAsColor = "drawAsColor"_hs;

constexpr entt::hashed_string p_ValueMin = "ValueMin"_hs;
constexpr entt::hashed_string p_ValueMax = "ValueMax"_hs;
constexpr entt::hashed_string p_SliderSpeed = "sliderSpeed"_hs;

// This function is shown in the Editor as a button
constexpr entt::hashed_string p_ShownInEditor = "showInEditor"_hs;

// Internals
// If a anything that is drawn is marked with this, Reflect will ignore it under all cases.
// Intend is that we do something special do with this and we want the systems to ignore it.)
// For instance we have some components that we create behind the scene and users should never know about or are shown in a
// custom manner (Namecomponent or HierarchyComponent)
constexpr entt::hashed_string p_Internal = "Internal"_hs;

//*****************************************
//***			COMPONENT               ***
//*****************************************

// property name shown to the user in the inspector
#define PROP_DISPLAYNAME(NAME) .prop(p_DisplayName, std::string(NAME))
#define PROP_DESCRIPTION(NAME) .prop(p_ToolTip, std::string(NAME))

// Only used for GLM::vec3
#define PROP_COLORPICKER .prop(p_DrawAsColor)
// Information shown to the user when hovering over the element
#define PROP_TOOLTIP(TEXT) .prop(p_ToolTip, TEXT)
// Clamping the value
#define PROP_MINMAX(MIN, MAX) .prop(p_ValueMin, MIN).prop(p_ValueMax, MAX)
// Hide this property from the inspector
#define PROP_HIDE .prop(p_HideFromInspector)
// User can see but not editor the value in the inspector
#define PROP_READONLY .prop(p_ReadOnlyInInspector)

#define PROP_DRAGSPEED(SPEED) .prop(p_SliderSpeed, SPEED)

#define PROP_NORMALIZED .prop(p_Normalized)
#define PROP_SHOWNINEDITOR .prop(p_ShownInEditor)

// Implementation details for these macros are defined in MetaReflectImplement.h
#ifdef __clang__
#define REFLECT()                 \
private:                          \
    static int InitTypeReflect(); \
    __attribute__((constructor)) static void initialize(void) { InitTypeReflect(); };
#else
#define REFLECT()                 \
private:                          \
    static int InitTypeReflect(); \
    static inline int id = InitTypeReflect();
#endif

#ifdef __clang__
#define REFLECT_ENUM(TYPE)                                                                \
    namespace ENUMREFLECTS::TYPE_##TYPE                                                   \
    {                                                                                     \
        int InitTypeReflect();                                                            \
        __attribute__((constructor)) static void initialize(void) { InitTypeReflect(); }; \
    }
#else
#define REFLECT_ENUM(TYPE)                 \
    namespace ENUMREFLECTS::TYPE_##TYPE    \
    {                                      \
        int InitTypeReflect();             \
        static int id = InitTypeReflect(); \
    }
#endif

#ifdef __clang__
// TODO
#else

#define META_INIT(TYPE)                           \
    namespace TYPE_REFLECTS::TYPE_##TYPE          \
    {                                             \
        int InitTypeReflect();                    \
        static inline int id = InitTypeReflect(); \
    }
#endif


META_INIT(void);