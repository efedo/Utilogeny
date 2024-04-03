#include <iostream>

#include "magnum_widget_internal.h"
#include <QWindow>
#include <QVBoxLayout>
#include <SDL.h>
#include <SDL_syswm.h>

int MagnumWidgetInternal::dummyarg = 0;

MagnumWidgetInternal::MagnumWidgetInternal() 
    : Magnum::Platform::Sdl2Application({ dummyarg, (char**)(nullptr) }) 
{
    SDL_Window* sdlwindow = this->Magnum::Platform::Sdl2Application::window();
    uint32_t sdl_window_id = SDL_GetWindowID(sdlwindow);

    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(sdlwindow, &wmInfo);
    HWND native_window_id = wmInfo.info.win.window;
    std::cout << "Native SDL window id: " << native_window_id << "\n";

    QWindow* window = QWindow::fromWinId((WId)(native_window_id));
    window->setFlags(Qt::FramelessWindowHint);
    QWidget* widget = QWidget::createWindowContainer(window);
    QVBoxLayout* layout = new QVBoxLayout(this);
    this->setLayout(layout);
    layout->setMargin(0);
    layout->addWidget(widget);

    // https://stackoverflow.com/questions/33699258/qt-5-5-embed-external-application-into-qwidget

    initializeGfx();

    //Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::DepthTest);
    //Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::FaceCulling);


}

//MagnumWidgetInternal::~MagnumWidgetInternal() {
    //if (magnumApp) {
    //    delete magnumApp;
    //    magnumApp = 0;
    //}
//}

//
//bool MagnumWidgetInternal::isInside(Magnum::Vector3 vec) {
//    return true;
//}
//
//void MagnumWidgetInternal::addCube(Magnum::Vector3 lowpos, Magnum::Color3 color, float size) {
//    //vertices.push_back(TriangleVertex{ lowpos + Magnum::Vector3{ 0.0f, 0.0f, 0.0f}, { 1.0f, 0.0f, 0.0f} });
//    //vertices.push_back(TriangleVertex{ lowpos + Magnum::Vector3{ 0.0f, 0.0f, 1.0f}, { 1.0f, 0.0f, 0.0f} });
//    //vertices.push_back(TriangleVertex{ lowpos + Magnum::Vector3{ 0.0f, 0.0f, 1.0f}, { 1.0f, 0.0f, 0.0f} });
//}

// external boundary


void MagnumWidgetInternal::initializeTriangle() {

    _mesh = std::make_unique<Magnum::GL::Mesh>();
    _shader = std::make_unique<Magnum::Shaders::VertexColor3D>(); //Shaders::Phong
    _buffer = std::make_unique<Magnum::GL::Buffer>();

    //_mesh = new Magnum::GL::Mesh;
    *_mesh = Magnum::MeshTools::compile(Magnum::Primitives::cubeSolid());

    //using namespace Magnum::Math::Literals;

    ////const TriangleVertex data[]{
    ////    {{-0.5f, -0.5f}, 0xff0000_rgbf},    /* Left vertex, red color */
    ////    {{ 0.5f, -0.5f}, 0x00ff00_rgbf},    /* Right vertex, green color */
    ////    {{ 0.0f,  0.5f}, 0x0000ff_rgbf}     /* Top vertex, blue color */
    ////};

    //std::array<TriangleVertex, 6> myTriangles =
    //{
    //    TriangleVertex{{-0.5f, -0.5f,  0.0f}, 0xff0000_rgbf},    /* Left vertex, red color */
    //    TriangleVertex{{ 0.5f, -0.5f,  0.1f}, 0x00ff00_rgbf},    /* Right vertex, green color */
    //    TriangleVertex{{ 0.0f,  0.5f,  0.0f}, 0x0000ff_rgbf},     /* Top vertex, blue color */
    //    TriangleVertex{{ 0.0f, -0.5f,  0.2f}, 0x0000ff_rgbf},    /* Left vertex, red color */
    //    TriangleVertex{{ 1.0f, -0.5f,  0.0f}, 0x0000ff_rgbf},    /* Right vertex, green color */
    //    TriangleVertex{{ 0.5f,  0.5f,  0.0f}, 0x0000ff_rgbf}     /* Top vertex, blue color */
    //};

    //Magnum::GL::Buffer buffer;
    //buffer.setData(myTriangles); //data

    //_mesh->setCount(6)
    //    .addVertexBuffer(std::move(buffer), 0,
    //        Magnum::Shaders::VertexColor3D::Position{},
    //        Magnum::Shaders::VertexColor3D::Color3{});
}

void MagnumWidgetInternal::initializeGfx() {
    initializeTriangle();
    std::cout << "Magnum graphics initialized?\n";
    *_shader = Magnum::Shaders::VertexColor3D();
}
void MagnumWidgetInternal::drawEvent() {
    //if (!this->isVisible()) return;
    Magnum::GL::Context::current().resetState(Magnum::GL::Context::State::ExitExternal);
    Magnum::GL::defaultFramebuffer.clear(Magnum::GL::FramebufferClear::Color);

    /* TODO: Add your drawing code here */
    _shader->draw(*_mesh);
    this->swapBuffers();
    Magnum::GL::Context::current().resetState(Magnum::GL::Context::State::EnterExternal);
}

void MagnumWidgetInternal::showEvent(QShowEvent*) {
    connect(&frameTimer, &QTimer::timeout, this, &MagnumWidgetInternal::drawEvent);
    frameTimer.setInterval(this->msperframe);
    frameTimer.start();
    //drawEvent();
    //drawThread = new std::thread(&MagnumWidgetInternalSDL::drawEvent, this);
    //drawThread->join();
}
