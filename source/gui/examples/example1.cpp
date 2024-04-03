#define SDL_MAIN_HANDLED

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include "Utilogeny/source/gui/lib/Application.h"
#include "Utilogeny/source/gui/lib/window/Window.h"
#include "Utilogeny/source/gui/lib/ExtendedException.h"
#include <iostream>
#include <SDL.h>

int main() {

    //DeadSimpleGUI++ // main library/implementation
    //DeadSimpleGUI // wrappers for other languages

    // Window window("My window", 1200, 800);
    // 
    // // Simple absolute and relative positioning of GUI elements
    // 
    // Button button1(window, 50, 50, 200, 50); // position relative to parent
    // Button button2(window, button1.left() + 50, 50, 200, 50); // left side of button 2 is initially aligned to left of button 1 + 50
    // Button button3(window, anc(button1.left() + 50), 50, 200, 50); // left side of button 3 is anchored to left of button 1 + 50
    // button1.move(50,0); // button 3 also moves by 50
    // 
    // N.b. rel is a lambda wrapper in this instance
    // 
    // // Simple 3D
    // 
    // View3d view3d(window, 200, 200, 200, 200); // default projection is relative to screen
    // Teapot teapot1(view3d, 50, 50, 50); // displays at 50x, 50y, 10z relative to view3d
    // Teapot teapot1(view3d, 0.25r, 0.25r, 0.25r); // displays at 50x, 50y, 10z relative to view3d
    // teapot1.set_color(Color::red); // teapot is now red
    // Teapot teapot2(view3d, view3d.width() * 0.25), view3d.height_anchor() * 0.25, rel(view3d.depth() * 10)); // displays at 50x, 50y, 10z relative to view3d
    // view3d resize
    // 
    // 
    // width = lambda or constant

    std::shared_ptr<spdlog::logger> stdout_logger = spdlog::stdout_color_mt("stdout");
    spdlog::set_default_logger(stdout_logger);

    auto console = spdlog::stdout_color_mt("console");

    try {

        spdlog::info("Starting Doodad");

        /*
        Currently supported combinations:
        GLFW / OpenGL
        GLFW / BGFX

        SDL / SDL
        SDL / OpenGL
        SDL / BGFX
        */

        setWindowingSystem(WindowingSystem::SDL);
        setRendererType(RendererType::BGFX);

        Window win1("Window #1", 800, 600);
        Window win2("Window #2", 800, 600);

        std::this_thread::sleep_for(std::chrono::seconds{ 1 });

        win2.setTitle("New window title");

        std::this_thread::sleep_for(std::chrono::seconds{ 1 });

        win2.setBackColor(RGBA8{ 0,255,0,255 });

        std::this_thread::sleep_for(std::chrono::seconds{ 1 });

        SDL_Window* sldwin1 = win1.getSdlWindow();
        SDL_Window* sldwin2 = win2.getSdlWindow();

        app.exec();

    }
    catch (const ExtendedException& e) {
        auto excepttxt = e.ewhat();
        std::cerr << "Caught ExtendedException: " << excepttxt << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Caught std::exception: " << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Caught an unknown exception." << std::endl;
    }

    spdlog::shutdown();

    return 0;
}