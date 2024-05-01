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

#include <stdio.h>
#include <string.h>

#include <EGL/eglplatform.h>

#include "GLEngine/Khr/Egl/Egl.hpp"

///
// GetContextRenderableType()
//
//    Check whether EGL_KHR_create_context extension is supported.  If so,
//    return EGL_OPENGL_ES3_BIT_KHR instead of EGL_OPENGL_ES2_BIT
//
EGLint GetContextRenderableType(EGLDisplay eglDisplay)
{
#ifdef EGL_KHR_create_context
   const char *extensions = eglQueryString ( eglDisplay, EGL_EXTENSIONS );

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

//////////////////////////////////////////////////////////////////
//
//  Public Functions
//
//

///
//  EGL_CreateWindow()
//
bool Egl::CreateSurfaceAndBindContext(EGLNativeDisplayType display, EGLNativeWindowType window, GLuint flags)
{
   EGLConfig config;
   EGLint majorVersion;
   EGLint minorVersion;
   EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };

#ifdef ANDROID
   // For Android, get the width/height from the window rather than what the
   // application requested.
   context->width = ANativeWindow_getWidth(display);
   context->height = ANativeWindow_getHeight(window);
#endif

   eglDisplay = eglGetDisplay(display);
   if ( eglDisplay == EGL_NO_DISPLAY )
   {
      return GL_FALSE;
   }

   // Initialize EGL
   if ( !eglInitialize ( eglDisplay, &majorVersion, &minorVersion ) )
   {
      return GL_FALSE;
   }

   printf("%s: EGL VERSION: %d.%d\n", __func__, majorVersion, minorVersion);

   {
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
         EGL_RENDERABLE_TYPE, GetContextRenderableType(eglDisplay),
         EGL_NONE
      };

      // Choose config
      if(!eglChooseConfig(eglDisplay, attribList, &config, 1, &numConfigs))
      {
         return GL_FALSE;
      }

      if(numConfigs < 1)
      {
         return GL_FALSE;
      }
   }

#ifdef ANDROID
   // For Android, need to get the EGL_NATIVE_VISUAL_ID and set it using ANativeWindow_setBuffersGeometry
   {
      EGLint format = 0;
      eglGetConfigAttrib ( eglDisplay, config, EGL_NATIVE_VISUAL_ID, &format );
      ANativeWindow_setBuffersGeometry ( context->eglNativeWindow, 0, 0, format );
   }
#endif // ANDROID

   // Create a surface
   eglSurface = eglCreateWindowSurface(eglDisplay, config, window, NULL);

   if(eglSurface == EGL_NO_SURFACE)
   {
      return GL_FALSE;
   }

   // Create a GL context
   eglContext = eglCreateContext(eglDisplay, config, EGL_NO_CONTEXT, contextAttribs);

   if(eglContext == EGL_NO_CONTEXT)
   {
      return GL_FALSE;
   }

   // Make the context current
   if(!eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext))
   {
      return GL_FALSE;
   }

   return GL_TRUE;
}

void Egl::swapBuffers() {
   // Swap buffers. It's a blocking call if VSYNC is enabled, in that case will wait 16ms.
   eglSwapBuffers(eglDisplay, eglSurface);
}