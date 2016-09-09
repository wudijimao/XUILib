#pragma once

#include "../../include/core/GLCanvas.hpp"

struct saved_state {
    float angle;
    int32_t x;
    int32_t y;
};

struct engine {
    struct android_app *app;

    ASensorManager *sensorManager;
    const ASensor *accelerometerSensor;
    ASensorEventQueue *sensorEventQueue;

    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    int32_t width;
    int32_t height;
    struct saved_state state;
};

class SIMPLEDIRECTUI_API_DEBUG GLCanvas_android : public GLCanvas {
public:
    ~GLCanvas_android();
    GLCanvas_android();
    bool init(struct engine *engine);
    virtual bool Present() override;

private:
    struct engine *mEngine;
};
