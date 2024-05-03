// The MIT License (MIT)
//
// Copyright (c) 2013 Dan Ginsburg, Budirijanto Purnomo
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

//
// Book:      OpenGL(R) ES 3.0 Programming Guide, 2nd Edition
// Authors:   Dan Ginsburg, Budirijanto Purnomo, Dave Shreiner, Aaftab Munshi
// ISBN-10:   0-321-93388-5
// ISBN-13:   978-0-321-93388-1
// Publisher: Addison-Wesley Professional
// URLs:      http://www.opengles-book.com
//            http://my.safaribooksonline.com/book/animation-and-3d/9780133440133
//
// ESUtil.c
//
//    A utility library for OpenGL ES.  This library provides a
//    basic common framework for the example applications in the
//    OpenGL ES 3.0 Programming Guide.
//

#include "GLEngine/Khr/Egl/Egl.hpp"

#include <stdio.h>
#include <string.h>

#include <EGL/eglplatform.h>

#include "GLEngine/Khr/Egl/EglWindowSystem.hpp"

#define CHECK_BOOL(___expression___) do{ if(!___expression___) { printf("%s: '%s' failed\n", __func__, #___expression___); return false; }} while(0)
#define CHECK_NOT(___expression___, ___cond___) do{ if(___expression___ == ___cond___) { printf("%s: '%s == %s' failed\n", __func__, #___cond___, #___expression___); return false; }} while(0)
#define CHECK_GRATER_THAN(___expression___, ___cond___) do{ if(___expression___ <= ___cond___) { printf("%s: '%s <= %s' failed\n", __func__, #___cond___, #___expression___); return false; }} while(0)

///
// GetContextRenderableType()
//
//    Check whether EGL_KHR_create_context extension is supported.  If so,
//    return EGL_OPENGL_ES3_BIT_KHR instead of EGL_OPENGL_ES2_BIT
//
EGLint GetContextRenderableType(EGLDisplay display)
{
#ifdef EGL_KHR_create_context
    const char *extensions = eglQueryString ( display, EGL_EXTENSIONS );

    // check whether EGL_KHR_create_context is in the extension string
    if ( extensions != NULL && strstr( extensions, "EGL_KHR_create_context" ) )
    {
        // extension is supported
        return EGL_OPENGL_ES3_BIT_KHR;
    }
#endif
    // extension is not supported
    return EGL_OPENGL_ES2_BIT;
}

bool Egl::attachToNativeDisplay() {
    windowSystem.attachToNativeDisplay();

    return true;
}

bool Egl::createNativeWindow(const char *title, int posx, int posy, int width, int height) {
    windowSystem.createNativeWindow(title, posx, posy, width, height);

    return true;
}

bool Egl::initEglOnDisplay() {
    display = eglGetDisplay(windowSystem.getNativeDisplay());
    CHECK_NOT(display, EGL_NO_DISPLAY);
    CHECK_BOOL(eglInitialize ( display, &majorVersion, &minorVersion ));
    
    printf("%s: EGL VERSION: %d.%d\n", __func__, majorVersion, minorVersion);

    return true;
}

bool Egl::getFramebufferConfig(GLuint flags) {
    EGLint numConfigs = 0;
    EGLint attribList[] =
    {
        EGL_RED_SIZE,       5,
        EGL_GREEN_SIZE,     6,
        EGL_BLUE_SIZE,      5,
        EGL_ALPHA_SIZE,     (flags & EGL_FLAG_FRAMEBUFFER_ALPHA) ? 8 : EGL_DONT_CARE,
        EGL_DEPTH_SIZE,     (flags & EGL_FLAG_FRAMEBUFFER_DEPTH) ? 8 : EGL_DONT_CARE,
        EGL_STENCIL_SIZE,   (flags & EGL_FLAG_FRAMEBUFFER_STENCIL) ? 8 : EGL_DONT_CARE,
        EGL_SAMPLE_BUFFERS, (flags & EGL_FLAG_FRAMEBUFFER_MULTISAMPLE) ? 1 : 0,
        // if EGL_KHR_create_context extension is supported, then we will use
        // EGL_OPENGL_ES3_BIT_KHR instead of EGL_OPENGL_ES2_BIT in the attribute list
        EGL_RENDERABLE_TYPE, GetContextRenderableType(display),
        EGL_NONE
    };

    // Choose config
    CHECK_BOOL(eglChooseConfig(display, attribList, &config, 1, &numConfigs));

    CHECK_GRATER_THAN(numConfigs, 0);

    return true;
}

bool Egl::createSurface() {
#ifdef EGL_USE_ANDROID
    // For Android, need to get the EGL_NATIVE_VISUAL_ID and set it using ANativeWindow_setBuffersGeometry
    {
        EGLint format = 0;
        eglGetConfigAttrib ( display, config, EGL_NATIVE_VISUAL_ID, &format );
        ANativeWindow_setBuffersGeometry ( context->eglNativeWindow, 0, 0, format );
    }
#endif // EGL_USE_ANDROID

    // Create a surface
    surface = eglCreateWindowSurface(display, config, windowSystem.getNativeWindow(), NULL);

    CHECK_NOT(surface, EGL_NO_SURFACE);

    return true;
}

bool Egl::createContext() {
    EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };

    context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);

    CHECK_NOT(context, EGL_NO_CONTEXT);

    return true;
}

bool Egl::bindContextToSurface() {
    // Make the context current
    CHECK_BOOL(eglMakeCurrent(display, surface, surface, context));

    return true;
}

//////////////////////////////////////////////////////////////////
//
//  Public Functions
//
//

///
//  EGL_CreateWindow()
//
bool Egl::CreateSurfaceAndBindContext(const char *title, int posx, int posy, int width, int height, GLuint flags)
{
    CHECK_BOOL(attachToNativeDisplay());
    CHECK_BOOL(createNativeWindow(title, posx, posy, width, height));
    CHECK_BOOL(initEglOnDisplay());
    CHECK_BOOL(getFramebufferConfig(flags));
    CHECK_BOOL(createSurface());
    CHECK_BOOL(createContext());
    CHECK_BOOL(bindContextToSurface());

    return true;
}

void Egl::swapBuffers() {
    // Swap buffers. It's a blocking call if VSYNC is enabled, in that case will wait 16ms.
    eglSwapBuffers(display, surface);
}

void Egl::getEvent(WindowEvent *event) const {
    windowSystem.getEvent(event);
}