WARNING THIS LIBRARY IS STILL UNDER CONSTRUCTION AND NOT READY FOR PRODUCTION YET.

# EnttEditor
Editor for entt libary, using their build in reflection system

# What do we add

We add a layer ontop of entt's build in Reflection system allowing you to use these types inline.

With support of properties we can draw types any way we want.

We also provide you with a framework to add reflect any custom type allowing you to easily add support for drawing your own types to Imgui.
And add custom properties support by the types.


# How to get Started.
Include `Entt` and `ImGui` in your project.

Include this project in your repo.

If you do not use `GLM` do not include `GlmMetaInspectors`

(Idealy you build your own high level window as it allows you to draw hierachies, But for sake of getting started quickly we provide a demo window !)
1. Create a instance of the `DemoWindow::DemoEntityWindow` found in `Examples/ExampleEntityWindow`, Pass in the registery. (Lifetime of the EntityWindow should not exceed the registeries) 
2. Call `DemoEntityWindow::Draw(true)`within imgui draw context.

This should show you a imgui window like shown below
![image](https://github.com/TheDimin/EnttEditor/assets/7473370/157da7a6-e5ee-4a6f-be62-5870d0a08d0d)


# how to show components
The `Examples/ExampleEntityWindow.h`contains an example `TransformComponent` and `NameComponent` which are fully reflected !

1. Within the body of the component Add the `REFLECT()` Macro. For this make sure to include `"EnttEditor/MetaReflect.h"`

![image](https://github.com/TheDimin/EnttEditor/assets/7473370/55fe2346-6184-446f-a984-574fa67b82d2)

2. in the cpp file we add the `IMPLEMENT_REFLECT_COMPONENT(<CLASS>)` and include `"EnttEditor/MetaReflectImplement.h"` (DO NOT INCLUDE THIS IN THE HEADER)
3. Give this a body and add the variables & functions with the entt::meta system.
We provide a few extra Defines to make your life easier, these can be found in `"EnttEditor/MetaReflect.h"`
A good example is the PROP_DISPLAYNAME or PROP_READONLY (PROP is used as aprefix for properties for data)

5. Make sure to finish with `FinishReflect()` macro.

Full example below:

![image](https://github.com/TheDimin/EnttEditor/assets/7473370/d6b282c6-fba7-4d8b-b3eb-29d21c96199c)
