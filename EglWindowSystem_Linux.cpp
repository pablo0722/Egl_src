#include "GLEngine/Khr/Egl/EglWindowSystem.hpp"

#include <stdio.h>
#include <string.h>
#include <sys/time.h>

void EglWindowSystem::attachToNativeDisplay() {
   windowSystem.attachToNativeDisplay();
   display = (EGLNativeDisplayType) windowSystem.getNativeDisplay();
}

void EglWindowSystem::createNativeWindow(const char *title, int posx, int posy, int width, int height) {
    windowSystem.createNativeWindow(title, posx, posy, width, height);
    window = (EGLNativeWindowType) windowSystem.getNativeWindow();
}

void EglWindowSystem::getEvent(WindowEvent *event) const
{
    windowSystem.getEvent(event);
}

EGLNativeDisplayType EglWindowSystem::getNativeDisplay() const {
    return display;
}

EGLNativeWindowType EglWindowSystem::getNativeWindow() const {
    return window;
}
