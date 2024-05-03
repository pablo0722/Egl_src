#include "GLEngine_src/Khr_src/Egl_src/EglWindowSystem.hpp"

#include <stdio.h>
#include <string.h>
#include <sys/time.h>

void EglWindowSystem::attachToNativeDisplay() {
}

void EglWindowSystem::createNativeWindow(const char *title, int posx, int posy, int width, int height) {
}

void EglWindowSystem::registerKeyFunc(void (*keyFunc)(void *ctx, unsigned char keyChar, int x, int y)) {
    this->keyFunc = keyFunc;
}

EglEvent EglWindowSystem::getEvent(void *ctx) const
{
    Event ret = Event::Empty;

    return ret;
}

EGLNativeDisplayType EglWindowSystem::getNativeDisplay() const {
    return display;
}

EGLNativeWindowType EglWindowSystem::getNativeWindow() const {
    return window;
}
