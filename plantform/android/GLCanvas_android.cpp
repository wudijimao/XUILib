//
// Created by ximiao on 16/7/8.
//

#include "GLCanvas_android.hpp"

/**
 * Initialize an EGL context for the current display.
 */
static int engine_init_display(struct engine *engine) {
    // initialize OpenGL ES and EGL

    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least 8 bits per color
     * component compatible with on-screen windows
     */
    const EGLint attribs[] = {
            //EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_ALPHA_SIZE, 8,
            EGL_DEPTH_SIZE, 0,
            EGL_STENCIL_SIZE, 8,
            EGL_NONE
    };
    EGLint w, h, dummy, format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, 0, 0);

    /* Here, the application chooses the configuration it desires.
     * find the best match if possible, otherwise use the very first one
     */
    eglChooseConfig(display, attribs, nullptr, 0, &numConfigs);
    auto supportedConfigs = new EGLConfig[numConfigs];
    assert(supportedConfigs);
    eglChooseConfig(display, attribs, supportedConfigs, numConfigs, &numConfigs);
    assert(numConfigs);
    auto i = 0;
    for (; i < numConfigs; i++) {
        auto &cfg = supportedConfigs[i];
        EGLint r, g, b, a, d, s;
        if (eglGetConfigAttrib(display, cfg, EGL_RED_SIZE, &r) &&
            eglGetConfigAttrib(display, cfg, EGL_GREEN_SIZE, &g) &&
            eglGetConfigAttrib(display, cfg, EGL_BLUE_SIZE, &b) &&
//            eglGetConfigAttrib(display, cfg, EGL_ALPHA_SIZE, &a) &&
            eglGetConfigAttrib(display, cfg, EGL_DEPTH_SIZE, &d) &&
            eglGetConfigAttrib(display, cfg, EGL_STENCIL_SIZE, &s) &&
            r == 8 && g == 8 && b == 8 /*&& a==8*/ && d == 0 && s == 8) {

            config = supportedConfigs[i];
            break;
        }
    }
    if (i == numConfigs) {
        config = supportedConfigs[0];
    }

    const EGLint contextAttribs[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2,
            EGL_NONE
    };


    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
    surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        return -1;
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    engine->display = display;
    engine->context = context;
    engine->surface = surface;
    engine->width = w;
    engine->height = h;
    engine->state.angle = 0;

    // Check openGL on the system
    auto opengl_info = {GL_VENDOR, GL_RENDERER, GL_VERSION, GL_EXTENSIONS};
    for (auto name : opengl_info) {
        auto info = glGetString(name);
    }

    return 0;
}


GLCanvas_android::~GLCanvas_android() {

}
GLCanvas_android::GLCanvas_android() {

}



bool GLCanvas_android::init(struct engine *engine) {

    mEngine = engine;
    engine_init_display(engine);
    XResource::XSize size(engine->width / 3.0f,engine->height / 3.0f);
    setSize(size, 3);

    const char* versionStr = (const char*)glGetString(GL_VERSION);
    if (strstr(versionStr, "OpenGL ES 3.") ) {
    } else if (strstr(versionStr, "OpenGL ES 2.")) {
    } else {
    }

//    glGenRenderbuffers(1, &_renderBuffer);
//    glBindRenderbuffer(GL_RENDERBUFFER, _renderBuffer);
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_SRGB8_ALPHA8, _size.Width(), _size.Height());
    if (!this->InitGLProgram()) {
        return false;
    }
//    if (!this->InitFrameBuffer()) {
//        return false;
//    }

    this->enableGLSettings();
    return true;
}

#include "../../include/core/GL/GLProgram.hpp"

bool GLCanvas_android::Present() {
    GLCanvas::Present();
//    GLuint gvPositionHandle = _program.getAttributeIndex("vPosition");// glGetAttribLocation(_program, "vPosition");
//    const GLfloat vertices[] = {
//            0.0f,  0.5f,
//            -0.5f, -0.5f,
//            0.5f, -0.5f
//    };
//
//    glClearColor(1.0f, 0.3f, 0.3f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT);
//    _program.enable();
//    glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, vertices);
//    glEnableVertexAttribArray(gvPositionHandle);
//    glDrawArrays(GL_TRIANGLES, 0, 3);

    eglSwapBuffers(mEngine->display, mEngine->surface);
}