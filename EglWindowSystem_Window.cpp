#include "GLEngine_src/Khr_src/Egl_src/WindowSystem.hpp"

#include <stdio.h>
#include <string.h>
#include <sys/time.h>

void WindowSystem::attachToNativeDisplay() {
}

void WindowSystem::createNativeWindow(const char *title, int posx, int posy, int width, int height) {
}

void WindowSystem::registerKeyFunc(void (*keyFunc)(void *ctx, unsigned char keyChar, int x, int y)) {
    this->keyFunc = keyFunc;
}

EglEvent WindowSystem::getEvent(void *ctx) const
{
    Event ret = Event::Empty;

    return ret;
}

EGLNativeDisplayType WindowSystem::getNativeDisplay() const {
    return display;
}

EGLNativeWindowType WindowSystem::getNativeWindow() const {
    return window;
}
