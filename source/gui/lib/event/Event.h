#pragma once
#include <cstdint>

class Event {
public:
    enum class Type : uint8_t {
        None,
        Quit,
        WindowResize,
        WindowMoved,
        WindowFocusGained,
        WindowFocusLost,
        WindowRefresh,
        WindowIconified,
        WindowRestored,
        WindowMaximized,
        //JoystickAxisMotion,
        //JoystickBallMotion,
        //JoystickHatMotion,
        //JoystickButtonDown,
        //JoystickButtonUp,
        //JoystickDeviceAdded,
        //JoystickDeviceRemoved,
        //ControllerAxisMotion,
        //ControllerButtonDown,
        //ControllerButtonUp,
        //ControllerDeviceAdded,
        //ControllerDeviceRemoved,
        //ControllerDeviceRemapped,
        MouseButtonDown,
        MouseButtonUp,
        MouseMotion,
        MouseWheel,
        KeyDown,
        KeyUp,
        TextInput,
        CmdSetWindowTitle,
        CmdSetWindowSize
    };

    struct WindowResizeEvent {
        int width;
        int height;
    };

    struct WindowMovedEvent {
        int x;
        int y;
    };

    struct WindowFocusEvent {
        bool focused;
    };

    struct WindowIconifiedEvent {
        bool iconified;
    };

    struct WindowMaximizedEvent {
        bool maximized;
    };

    struct JoystickAxisMotionEvent {
        int deviceIndex;
        int axis;
        int16_t value;
    };

    struct JoystickBallMotionEvent {
        int deviceIndex;
        int ball;
        int16_t xrel;
        int16_t yrel;
    };

    struct JoystickHatMotionEvent {
        int deviceIndex;
        int hat;
        uint8_t value;
    };

    struct JoystickButtonEvent {
        int deviceIndex;
        uint8_t button;
        bool pressed;
    };

    struct JoystickDeviceEvent {
        int deviceIndex;
    };

    struct ControllerAxisMotionEvent {
        int deviceIndex;
        int axis;
        int16_t value;
    };

    struct ControllerButtonEvent {
        int deviceIndex;
        uint8_t button;
        bool pressed;
    };

    struct ControllerDeviceEvent {
        int deviceIndex;
    };

    struct MouseButtonEvent {
        int x;
        int y;
        uint8_t button;
    };

    struct MouseMotionEvent {
        int x;
        int y;
        int xrel;
        int yrel;
    };

    struct MouseWheelEvent {
        int x;
        int y;
    };

    struct KeyEvent {
        uint32_t keycode;
        uint16_t scancode;
        uint16_t mod;
    };

    struct TextInputEvent {
        char text[32];
    };

    struct CmdSetWindowTitle {
        char title[256];
    };

    struct CmdSetWindowSize {
        int width;
        int height;
    };

    Event() : type(Type::None) {}

    Type type;
    union {
        WindowResizeEvent windowResize;
        WindowMovedEvent windowMoved;
        WindowFocusEvent windowFocus;
        WindowIconifiedEvent windowIconified;
        WindowMaximizedEvent windowMaximized;
        JoystickAxisMotionEvent joystickAxisMotion;
        JoystickBallMotionEvent joystickBallMotion;
        JoystickHatMotionEvent joystickHatMotion;
        JoystickButtonEvent joystickButton;
        JoystickDeviceEvent joystickDevice;
        ControllerAxisMotionEvent controllerAxisMotion;
        ControllerButtonEvent controllerButton;
        ControllerDeviceEvent controllerDevice;
        MouseButtonEvent mouseButton;
        MouseMotionEvent mouseMotion;
        MouseWheelEvent mouseWheel;
        KeyEvent key;
        TextInputEvent textInput;
        CmdSetWindowTitle setWindowTitle;
        CmdSetWindowSize setWindowSize;
    };
};