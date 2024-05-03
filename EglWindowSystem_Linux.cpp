#include "GLEngine_src/Khr_src/Egl_src/EglWindowSystem.hpp"

#include <stdio.h>
#include <string.h>
#include <sys/time.h>

void WindowSystem::attachToNativeDisplay() {
   windowSystem.attachToNativeDisplay();
   display = (EGLNativeDisplayType) windowSystem.getNativeDisplay();
}

void WindowSystem::createNativeWindow(const char *title, int posx, int posy, int width, int height) {
    windowSystem.createNativeWindow(title, posx, posy, width, height);
    window = (EGLNativeWindowType) windowSystem.getNativeWindow();
}

void WindowSystem::getEvent(WindowEvent *event) const
{
    windowSystem.getEvent(event);
}

EGLNativeDisplayType WindowSystem::getNativeDisplay() const {
    return display;
}

EGLNativeWindowType WindowSystem::getNativeWindow() const {
    return window;
}
