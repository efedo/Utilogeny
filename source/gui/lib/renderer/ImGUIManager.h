#pragma once
#include <mutex>
#include "imgui.h"

// None of the included functions (other than lock) are context/thread-safe. 
// Make sure you lock the ImGUI context before calling
class ImGuiContextManager {
public:
	ImGuiContextManager();
	~ImGuiContextManager();
	void sharedInit();
	void loadFonts();
	void testDraw();
	float getFrameBufferXScale();
	float getFrameBufferYScale();
	int getDisplaySizeX();
	int getDisplaySizeY();
	void registerCallbacks();

	std::unique_lock<std::mutex> lockImgui(); // Prevents multiple contexts from intermingling ImGUI calls
private:
	static std::mutex imguiMtx;
	ImGuiContext* m_imGuiContext = 0;
	ImGuiIO * io = 0;
};