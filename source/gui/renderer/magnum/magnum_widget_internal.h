#pragma once

#include <memory>
#include <thread>

#include <Corrade/Containers/Optional.h>
/* Magnum GL headers always have to be included before Qt ones */
#include <Magnum/GL/Framebuffer.h>
#include <Magnum/Platform/GLContext.h>
#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Shaders/VertexColor.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/MeshTools/Interleave.h>
#include <Magnum/MeshTools/CompressIndices.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Shaders/Phong.h>
#include <Magnum/Trade/MeshData.h>

#include <QtWidgets/QApplication>
#include <QtWidgets/QOpenGLWidget>
#include <QTimer>

//#include "../generic/engine_widget_internal.h"
#include "../generic/engine_widget.h"

struct TriangleVertex {
    Magnum::Vector3 position;
    Magnum::Color3 color;
};

class MagnumWidgetInternal : public EngineWidgetInternal, public Magnum::Platform::Sdl2Application {
public:
    MagnumWidgetInternal();
protected:
    void initializeGfx();
    void addCube(Magnum::Vector3 lowpos, Magnum::Color3 color, float size);
    void initializeTriangle();
    std::unique_ptr<Magnum::GL::Mesh> _mesh;
    std::unique_ptr<Magnum::Shaders::VertexColor3D> _shader; //Shaders::Phong
    std::unique_ptr<Magnum::GL::Buffer> _buffer;
    Magnum::Matrix4 _transformation;
    Magnum::Matrix4 _projection;
    //Magnum::Color3 _color;
    //std::vector<TriangleVertex> vertices;
private:
    void drawLoop();
    std::thread* drawThread = 0;
    void drawEvent();
    void showEvent(QShowEvent*) override;
    static int dummyarg; // only here to provide a dummy lvalue for magnum sdl2application arguments
    QTimer frameTimer;
    float msperframe = 1000.0f / 60.0f;
};