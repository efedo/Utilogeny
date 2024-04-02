// SDLRenderer.cpp

#include "SDLRenderer.h"
#include "source/renderer/ImGUIManager.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer.h"
#include <stdio.h>
#include <SDL.h>
#include <iostream>
#include "source/window/Window.h"
#include "source/ExtendedException.h"

// https://github.com/ocornut/imgui/blob/master/examples/example_sdl2_sdlrenderer/main.cpp

SDLRenderer::SDLRenderer(Window* window)
    : Renderer(window)
{
    if (!WindowSDL()) THROW_EXTENDED_EXCEPTION("SDL renderer requires SDL window");

    m_renderer = SDL_CreateRenderer(getSdlWindow(), -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    
    if (m_renderer == nullptr) THROW_EXTENDED_EXCEPTION("Error creating SDL_Renderer!");

    //SDL_RendererInfo info;
    //SDL_GetRendererInfo(renderer, &info);
    //SDL_Log("Current SDL_Renderer: %s", info.name);
}

SDLRenderer::~SDLRenderer() {
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    if (m_imguiContextMngr) delete m_imguiContextMngr;
    SDL_DestroyRenderer(m_renderer);
}

void SDLRenderer::initialize() {
    std::cout << "Initializing SDL Renderer\n";
    setBackColor(RGBAf{ 0.1f, 0.5f, 0.1f, 0.5f });
    imguiInit();
}

void SDLRenderer::imguiInit() {
    m_imguiContextMngr = new ImGuiContextManager;

    auto imguiLock = m_imguiContextMngr->lockImgui();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(getSdlWindow(), m_renderer);
    ImGui_ImplSDLRenderer_Init(m_renderer);

    m_imguiContextMngr->loadFonts();
}

void SDLRenderer::clear() {
    RGBAf fltcolor = backColor;
    SDL_SetRenderDrawColor(m_renderer, static_cast<Uint8>(fltcolor.r * 255), static_cast<Uint8>(fltcolor.g * 255),
        static_cast<Uint8>(fltcolor.b * 255), static_cast<Uint8>(fltcolor.a * 255));
    SDL_RenderClear(m_renderer);
}

void SDLRenderer::render() {
    // Render here
    renderImgui();
}

void SDLRenderer::renderImgui() {
    auto lock = m_imguiContextMngr->lockImgui();

    // Start the Dear ImGui frame
    ImGui_ImplSDLRenderer_NewFrame();
    ImGui_ImplSDL2_NewFrame();

    m_imguiContextMngr->testDraw();

    SDL_RenderSetScale(m_renderer, m_imguiContextMngr->getFrameBufferXScale(), m_imguiContextMngr->getFrameBufferYScale());
    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(m_renderer);
}

void SDLRenderer::swapBuffers() {
    SDL_RenderPresent(m_renderer);
}

void SDLRenderer::makeCurrent() {

}

void SDLRenderer::drawRect(Rect rect, RGBA8 color) {
    setDrawColor(color.r, color.g, color.b, color.a);
    fillRect(rect);
}

void SDLRenderer::setDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
}

void SDLRenderer::fillRect(const Rect& rect) {
    SDL_Rect sdlRect = { rect.x, rect.y, rect.w, rect.h };
    SDL_RenderFillRect(m_renderer, &sdlRect);
}