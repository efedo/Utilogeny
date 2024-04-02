#include "BgfxRenderer.h"
#include <bgfx/bgfx.h>
#include <thread>
#include "source/event/polym/Queue.h"

class BgfxRendererContext;

class BgfxRendererImpl {
public:
    BgfxRendererImpl(BGFXRenderer*);
    ~BgfxRendererImpl();
    //void initGlobal();
    void initialize();
    void imguiInit();
    void render();
    void renderImgui();
    void swapBuffers();
    void setViewClear();
    void setBackColor(RGBA8 color);
private:
    BGFXRenderer* const m_renderer;
    void* const m_handle = 0;
    uint16_t m_view = 0;
    uint16_t m_width = 0;
    uint16_t m_height = 0;
    uint32_t m_debug = 0;
    uint32_t m_reset = 0;
    //bool m_primary_window = true; // bgfx treats main window differently; comes with its own framebuffer
    bgfx::FrameBufferHandle m_fbh;
    static BgfxRendererContext * m_context;
};

//entry::WindowState m_windows[MAX_WINDOWS];
//bgfx::FrameBufferHandle m_fbh[MAX_WINDOWS];
//InputBinding* m_bindings;
