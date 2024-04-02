*** Currently in early development!!! ***

Abstractorizer is thin wrapper to embed external GUI/3D graphics library widgets into Qt/CopperSpice.
Its goal is to allow the gradually migration of Qt widget/Qt3D applications to other frameworks, one widget
at a time. Where possible, it wraps libraries' native windows. Otherwise it embeds libraries in
an intermediate SDL window/widget, either directly or through a GPU context.

Once all widgets in an application are 'Abstractorized', you can then switch to CopperSpice or another 
layout/window manager of your choice with minimal effort and remove Abstractorizer from your application.

Contributions are very welcome, especially if you would like to help speed inclusion of a specific 
GUI library/3D engine.

---

Build Instructions

Optional dependencies:

Qt
Installed via vcpkg if required.

CopperSpice
Must be installed manually, no vcpkg/conan package available. Binaries available here: https://download.copperspice.com/copperspice/binary/

ImGUI
Bundled submodule.

LVGL
Bundled submodule.

Magnum
Installed via vcpkg if required.

Godot
Bundled submodule.

Wicked Engine
Bundled submodule.


---

To be considered for inclusion, a GUI library/3D engine should ideally:
- Be cross-platform
- Be useable as a library via a C++ API (either directly or with minimal tinkering)
- Have decent documentation
- Be either buildable or readily configurable via CMake
- Be under active development

Currently included:
- Magnum
- Qt3D
- SDL
- ImGUI
- tinyrenderer

In progress:
- Lvgl (waiting on resolution of MSVC bug # 1380599, until then adding a C++ interface is too painful)

Queued for inclusion:
- CopperSpice
- NanoGUI
- Nuklear
- OGRE classic (i.e. OGRE 1)
- OGRE-next  (i.e. OGRE 2)
- BGFX
- Panda3D
- zauonlok renderer (https://github.com/zauonlok/renderer)
- Quartz (https://github.com/Nadrin/Quartz)

Under consideration:
- Godot: if the editor dependency can be worked-around
- Filament
- Diligent Engine
- The Forge
- Yocto/GL
- GTK (widget toolkit only, if anyone has a working SDL-rendering backend)

On hold:
- Wicked Engine: no primitives library or dynamic mesh construction, limited documentation

Rejected:
- QtQuick3D: no public C++ API
- Urho3D: limited documentation, CMake build is a mess, project may be dead?
- Flutter (desktop support currently in beta; Dart)
	- https://codelabs.developers.google.com/codelabs/flutter-github-graphql-client/index.html#0
- Avalonia (.Net/C#)