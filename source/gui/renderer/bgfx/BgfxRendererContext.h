#pragma once

#include <bgfx/bgfx.h>
#include <memory>
#include "source/event/polym/Queue.h"
#include <thread>

class Window;

struct BgfxFBSetup {
    uint16_t height = 0;
    uint16_t width = 0;
    void* handle = 0;
    bgfx::FrameBufferHandle* fbh = 0;
};

enum class BgfxContextSignal : int {
    failure = 0,
    success,
    init,
    setup_fb,
    frame,
    terminate
};

class BgfxRendererContext {
public:
    BgfxRendererContext(); // todo: needs to start m_loop
    ~BgfxRendererContext(); // todo: needs to stop m_loop
    bgfx::VertexBufferHandle m_vbh;
    bgfx::IndexBufferHandle m_ibh;
    bgfx::ProgramHandle m_program;
    int32_t m_maxDim = 0;
    int32_t m_numThreads = 0;
    int32_t m_maxThreads = 0;
    bool m_used_views[100] = { 0 }; // max windows
    bool bgfx_init = false;
    std::unique_ptr<std::thread> m_loop = 0;
    PolyM::Queue m_queue;
private:
    //void* m_init_handle;
    std::unique_ptr<Window> m_dummy_main_window = 0;
    void init();
    void loop(); // needs to wait
};
