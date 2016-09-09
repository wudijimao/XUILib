//
// Created by ximiao on 16/7/8.
//

#include "../../include/core/XApp.hpp"
#include "../../include/core/GL/GLHeaders.h"
#include "XWindow_android.hpp"
#include "XMainRunloop_android.hpp"

//暂时  之后弃用struct engine
#include "GLCanvas_android.hpp"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

/**
 * Our saved state data.
 */
//struct saved_state {
//    float angle;
//    int32_t x;
//    int32_t y;
//};

/**
 * Shared state for our app.
 */
//struct engine {
//    struct android_app *app;
//
//    ASensorManager *sensorManager;
//    const ASensor *accelerometerSensor;
//    ASensorEventQueue *sensorEventQueue;
//
//    EGLDisplay display;
//    EGLSurface surface;
//    EGLContext context;
//    int32_t width;
//    int32_t height;
//    struct saved_state state;
//};




/**
 * Tear down the EGL context currently associated with the display.
 */
static void engine_term_display(struct engine *engine) {
    if (engine->display != EGL_NO_DISPLAY) {
        eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (engine->context != EGL_NO_CONTEXT) {
            eglDestroyContext(engine->display, engine->context);
        }
        if (engine->surface != EGL_NO_SURFACE) {
            eglDestroySurface(engine->display, engine->surface);
        }
        eglTerminate(engine->display);
    }
    engine->display = EGL_NO_DISPLAY;
    engine->context = EGL_NO_CONTEXT;
    engine->surface = EGL_NO_SURFACE;
}

/**
 * Process the next input event.
 */
static int32_t engine_handle_input(struct android_app *app, AInputEvent *event) {
    struct engine *engine = (struct engine *) app->userData;
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        engine->state.x = AMotionEvent_getX(event, 0);
        engine->state.y = AMotionEvent_getY(event, 0);
        return 1;
    }
    return 0;
}

/**
 * Process the next main command.
 */
static void engine_handle_cmd(struct android_app *app, int32_t cmd) {
    struct engine *engine = (struct engine *) app->userData;
    switch (cmd) {
        case APP_CMD_SAVE_STATE:
            // The system has asked us to save our current state.  Do so.
            engine->app->savedState = malloc(sizeof(struct saved_state));
            *((struct saved_state *) engine->app->savedState) = engine->state;
            engine->app->savedStateSize = sizeof(struct saved_state);
            break;
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            if (engine->app->window != NULL) {
                auto androidWindow = std::dynamic_pointer_cast<XWindow_android>(XDUILib::XApp::thisApp().mainWindow());
                androidWindow->init(engine);
            }
            break;
        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
            engine_term_display(engine);
            break;
        case APP_CMD_GAINED_FOCUS:
            // When our app gains focus, we start monitoring the accelerometer.
            if (engine->accelerometerSensor != NULL) {
                ASensorEventQueue_enableSensor(engine->sensorEventQueue,
                                               engine->accelerometerSensor);
                // We'd like to get 60 events per second (in us).
                ASensorEventQueue_setEventRate(engine->sensorEventQueue,
                                               engine->accelerometerSensor,
                                               (1000L / 60) * 1000);
            }
            break;
        case APP_CMD_LOST_FOCUS:
            // When our app loses focus, we stop monitoring the accelerometer.
            // This is to avoid consuming battery while not being used.
            if (engine->accelerometerSensor != NULL) {
                ASensorEventQueue_disableSensor(engine->sensorEventQueue,
                                                engine->accelerometerSensor);
            }
            break;
    }
}


namespace XDUILib {

    int XApp::run(struct android_app *state) {
        _thisApp = this;
        init();
        if (_mainWindow == nullptr) {
            return -1;
        }


        this->internalInit();
        initMainRunloop();

        struct engine engine;

        // Make sure glue isn't stripped.
        //app_dummy();

        memset(&engine, 0, sizeof(engine));
        state->userData = &engine;
        state->onAppCmd = engine_handle_cmd;
        state->onInputEvent = engine_handle_input;
        engine.app = state;

        // Prepare to monitor accelerometer
        engine.sensorManager = ASensorManager_getInstance();
        engine.accelerometerSensor = ASensorManager_getDefaultSensor(
                engine.sensorManager,
                ASENSOR_TYPE_ACCELEROMETER);
        engine.sensorEventQueue = ASensorManager_createEventQueue(
                engine.sensorManager,
                state->looper, LOOPER_ID_USER,
                NULL, NULL);

        if (state->savedState != NULL) {
            // We are starting with a previous saved state; restore from it.
            engine.state = *(struct saved_state *) state->savedState;
        }


        // loop waiting for stuff to do.

        // Read all pending events.
        int ident;
        int events;
        struct android_poll_source *source;

        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while (1) {
            while ((ident = ALooper_pollAll(0, NULL, &events,
                                            (void **) &source)) >= 0) {

                // Process this event.
                if (source != NULL) {
                    source->process(state, source);
                }

                // If a sensor has data, process it now.
                if (ident == LOOPER_ID_USER) {
                    if (engine.accelerometerSensor != NULL) {
                        ASensorEvent event;
                        while (ASensorEventQueue_getEvents(engine.sensorEventQueue,
                                                           &event, 1) > 0) {
                            LOGI("accelerometer: x=%f y=%f z=%f",
                                 event.acceleration.x, event.acceleration.y,
                                 event.acceleration.z);
                        }
                    }
                }

                // Check if we are exiting.
                if (state->destroyRequested != 0) {
                    engine_term_display(&engine);
                    return -1;
                }
            }
        }
        return 0;
    }
}