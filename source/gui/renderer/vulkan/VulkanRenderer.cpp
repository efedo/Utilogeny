// OpenGLRenderer.cpp

/*

#include "VulkanRenderer.h"
#include <SDL.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include "../window/Window.h"
#include <iostream>
#include "../ExtendedException.h"
#include <vector>

// Vulkan specific members and methods
struct VulkanStateStruct {
    VkInstance m_instance;
    VkPhysicalDevice m_physicalDevice;
    VkDevice m_device;
    VkQueue m_graphicsQueue;
    VkSurfaceKHR m_surface;
    VkSwapchainKHR m_swapChain;
    std::vector<VkImage> m_swapChainImages;
    VkFormat m_swapChainImageFormat;
    VkExtent2D m_swapChainExtent;
    std::vector<VkImageView> m_swapChainImageViews;
    VkRenderPass m_renderPass;
    VkPipeline m_graphicsPipeline;
    VkPipelineLayout m_pipelineLayout;
    std::vector<VkFramebuffer> m_swapChainFramebuffers;
    VkCommandPool m_commandPool;
    std::vector<VkCommandBuffer> m_commandBuffers;
    std::vector<VkSemaphore> m_imageAvailableSemaphores;
    std::vector<VkSemaphore> m_renderFinishedSemaphores;
};

VulkanRenderer::VulkanRenderer(Window* window)
    : Renderer(window)
{
    vk_state = new VulkanStateStruct;
}

VulkanRenderer::~VulkanRenderer() {
    //if ((m_window->getWindowingSystem() == WindowingSystem::SDL) && (m_glContext != 0)) {
    //    SDL_GL_DeleteContext(m_glContext);
    //}
    if (vk_state) delete vk_state;
}

void VulkanRenderer::initialize() {
    std::cout << "Initializing Vulkan renderer\n";

    if (m_window->getWindowingSystem() == WindowingSystem::SDL) {
        initializeSDL();
    }
    else if (m_window->getWindowingSystem() == WindowingSystem::GLFW) {
        initializeGLFW();
    }
}

void VulkanRenderer::initializeSDL() {

    // Load Vulkan library
    if (!SDL_Vulkan_LoadLibrary(nullptr)) {
        // Handle library loading error
    }

    // Get required Vulkan instance extensions
    unsigned int extensionCount;
    if (!SDL_Vulkan_GetInstanceExtensions((SDL_Window*)m_window->getNativeHandle(), &extensionCount, nullptr)) {
        // Handle extension retrieval error
    }

    const char** extensions = new const char* [extensionCount];
    if (!SDL_Vulkan_GetInstanceExtensions((SDL_Window*)m_window->getNativeHandle(), &extensionCount, extensions)) {
        // Handle extension retrieval error
    }

    // Create Vulkan instance with required extensions
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan Renderer";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = extensionCount;
    createInfo.ppEnabledExtensionNames = extensions;

    VkInstance instance;
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        // Handle instance creation error
    }

    // Create Vulkan surface
    VkSurfaceKHR surface;
    if (!SDL_Vulkan_CreateSurface((SDL_Window*)m_window->getNativeHandle(), instance, &surface)) {
        // Handle surface creation error
    }

    // Set up the Vulkan physical device, logical device, swapchain, and other required resources
    // Render using Vulkan API
}

void VulkanRenderer::clear() {
    RGBAf fltcolor = backColor;
    glClearColor(fltcolor.r, fltcolor.g, fltcolor.b, fltcolor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void VulkanRenderer::render() {
    glFlush();  // Flush the buffer
}

void VulkanRenderer::swapBuffers() {
    // May need to make current first

    void* native = m_window->getNativeHandle();
    WindowingSystem type = m_window->getWindowingSystem();

    if (type == WindowingSystem::SDL) {
        SDL_GL_SwapWindow(static_cast<SDL_Window*>(native));
    }
    else if (type == WindowingSystem::GLFW) {
        glfwSwapBuffers(static_cast<GLFWwindow*>(native));
    }
    else {
        std::cerr << "Unknown window type" << std::endl;
    }
}

void VulkanRenderer::makeCurrent() {
    //void* native = m_window->getNativeHandle();
    //if (native == m_currentNativeWindow) return;

    //WindowingSystem type = m_window->getWindowingSystem();

    //if (type == WindowingSystem::SDL) {
    //    if (SDL_GL_MakeCurrent(static_cast<SDL_Window*>(native), m_glContext) != 0) {
    //        // Handle context setting error
    //    }
    //}
    //else if (type == WindowingSystem::GLFW) {
    //    glfwMakeContextCurrent(static_cast<GLFWwindow*>(native));
    //}
    //else {
    //    std::cerr << "Unknown window type" << std::endl;
    //}

    //m_currentNativeWindow = native;
}

void VulkanRenderer::drawRect(Rect rect, RGBA8 color) {
    setDrawColor(color.r, color.g, color.b, color.a);
    fillRect(rect);
}

void VulkanRenderer::setDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    glColor4ub(r, g, b, a);
}

void VulkanRenderer::fillRect(const Rect& rect) {
    glBegin(GL_QUADS);
    glVertex2i(rect.x, rect.y);
    glVertex2i(rect.x + rect.w, rect.y);
    glVertex2i(rect.x + rect.w, rect.y + rect.h);
    glVertex2i(rect.x, rect.y + rect.h);
    glEnd();
}
*/