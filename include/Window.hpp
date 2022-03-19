#pragma once

#include <GLFW/glfw3.h>

typedef void (*frame_buffer_size_cb)(GLFWwindow* window, int w, int h);
typedef void (*process_input_cb)(GLFWwindow* window);
typedef void (*process_display_cb)();

struct Window {
    GLFWwindow* window;
    unsigned int width = 640;
    unsigned int height = 640;
    unsigned int frames = 0;

    void static on_close(GLFWwindow* window);
    void static err_on_callback(int err, const char* err_message);

    Window(unsigned int w, unsigned int h)
        : width(w)
        , height(h)
    {
    }

    ~Window();

    bool init(frame_buffer_size_cb resize_cb, process_input_cb input_cb, process_display_cb display_cb);

    void display();
    void set_resolution(unsigned int w, unsigned int h)
    {
        width = w;
        height = h;
    }
};
