#include "BgfxRendererContext.h"
#include <bgfx/platform.h>
#include <bx/bx.h>
#include <bx/math.h>
#include "spdlog/spdlog.h"
#include "source/window/Window.h"

BgfxRendererContext::BgfxRendererContext()
{
    m_dummy_main_window = std::make_unique<Window>("Dummy BGFX main window,", 1, 1, true);
    m_loop.reset(new std::thread(&BgfxRendererContext::loop, this));
}

BgfxRendererContext::~BgfxRendererContext() {
    m_queue.request(PolyM::Msg(int(BgfxContextSignal::terminate)));
    if (m_loop) m_loop->join();
}

void BgfxRendererContext::init() {
    // context locked by caller

    spdlog::info("Creating BGFX global context");

    // Set up bgfx platform with the window's native window handle
    bgfx::PlatformData pd{};
    pd.nwh = m_dummy_main_window->getNativeHandle();
    //pd.ndt = entry::getNativeDisplayHandle();
    //pd.type = entry::getNativeWindowHandleType();
    bgfx::setPlatformData(pd);

    // Initialize bgfx
    bgfx::Init init;
    init.type = bgfx::RendererType::Count; // Automatically choose renderer
    //init.vendorId = args.m_pciId;
    init.resolution.reset = BGFX_RESET_VSYNC; //m_reset;
    init.platformData = pd;
    if (!bgfx::init(init)) {
        spdlog::error("BGFX failed to initialize.");
    }

    // Get bgfx capabilities
    const bgfx::Caps* caps = bgfx::getCaps();
    m_maxDim = (int32_t)bx::pow(float(caps->limits.maxDrawCalls), 1.0f / 3.0f);
    m_maxThreads = caps->limits.maxEncoders;
    m_numThreads += 1; // should also decrement on destructor

    // init imGUI
    //imguiInit();

    // Done bgfx init
    bgfx_init = true;
}

void BgfxRendererContext::loop() {

    bool cont_loop = true;
    while (cont_loop) {

        auto ptr = m_queue.get();
        BgfxContextSignal sig = (BgfxContextSignal)(ptr->getMsgId());

        if (sig == BgfxContextSignal::init) {
            if (!bgfx_init) init(); // do global init if required
            m_queue.respondTo(ptr->getUniqueId(), PolyM::Msg(1));
        }
        else if (sig == BgfxContextSignal::setup_fb) {
            auto& dm = static_cast<PolyM::DataMsg<BgfxFBSetup>&>(*ptr);
            BgfxFBSetup fb_setup = dm.getPayload();
            *(fb_setup.fbh) = bgfx::createFrameBuffer(fb_setup.handle, fb_setup.width, fb_setup.height);
            m_queue.respondTo(ptr->getUniqueId(), PolyM::Msg(1));
        }
        else if (sig == BgfxContextSignal::frame) {
            bgfx::frame();
            m_queue.respondTo(ptr->getUniqueId(), PolyM::Msg(1));
        }
        else if (sig == BgfxContextSignal::terminate) {
            m_queue.respondTo(ptr->getUniqueId(), PolyM::Msg(1));
            cont_loop = false;
        }

        

    }
}