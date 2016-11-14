//
// Created by ximiao on 16/7/8.
//

#include "../../include/core/XApp.hpp"
#include "../../include/core/GL/GLHeaders.h"
#include "XWindow_android.hpp"
#include "XMainRunloop_android.hpp"

//暂时  之后弃用struct engine
#include "XAndroidEnv.hpp"
#include "JNIBridgeForNativeActivity.hpp"
#include "../../../Library/Android/sdk/ndk-bundle/sources/android/native_app_glue/android_native_app_glue.h"
#include "../../include/core/UIResponder.hpp"

#include <vector>
#include <memory>

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


void android_created_at_main_thread(struct android_app* app) {
    //initJNIActivityBridge(app);
}

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

static struct engine *gEngine;

/**
 * Process the next input event.
 */
static int32_t engine_handle_input(struct android_app *app, AInputEvent *event) {
    struct engine *engine = (struct engine *) app->userData;
    gEngine = engine;
    auto androidWindow = std::dynamic_pointer_cast<XWindow_android>(
            XDUILib::XApp::thisApp().mainWindow());

    int32_t lEventType = AInputEvent_getType(event);
    switch (lEventType) {
        case AINPUT_EVENT_TYPE_MOTION:
            //motion类型的消息的来源有两种，所以要获取消息的来源
            switch (AInputEvent_getSource(event)) {

                case AINPUT_SOURCE_TOUCHSCREEN: {//消息来源于触摸屏
                    auto touch = std::make_shared<XTouch>();
                    int32_t id = AMotionEvent_getAction(event);
                    switch (id) {
                        case AMOTION_EVENT_ACTION_MOVE:  //触摸移动消息
                            touch->phase = TouchPhase::Moved;
                            break;
                        case AMOTION_EVENT_ACTION_DOWN:   //触摸按下消息
                            touch->phase = TouchPhase::Began;
                            break;
                        case AMOTION_EVENT_ACTION_UP:   //触摸弹起消息
                            touch->phase = TouchPhase::Ended;
                            break;
                        default:
                            touch->phase = TouchPhase::Cancelled;
                            break;
                    }

                    touch->mPosition.X(AMotionEvent_getX(event, 0) / 3.0);
                    touch->mPosition.Y(AMotionEvent_getY(event, 0) / 3.0);
                    androidWindow->input(touch);
                    androidWindow->dispatchTouchs();
                }
                    break;
                case AINPUT_SOURCE_TRACKBALL: { //消息来源于trackball
                }
                    break;
            }

        case AINPUT_EVENT_TYPE_KEY: { //消息来源于物理键盘或虚拟键盘，这个处理是一样的
            if (XUI::UIResponder::sFirstResponder == nullptr) {
                break;
            }
            switch (AKeyEvent_getAction(event)) {
                case AKEY_EVENT_ACTION_DOWN: {
                    char text[2];
                    text[0] = AKeyEvent_getKeyCode(event);
                    text[1] = '\0';
                    switch (text[0]) {
                        case 0x04: {
                            std::vector<std::shared_ptr<XKeyInput>> inputs;
                            auto input = std::make_shared<XKeyInput>();
                            input->eventType = KeyEventType::Down;
                            input->eventButton = KeyEventButton::BackForward;
                            inputs.push_back(input);
                            XUI::UIResponder::sFirstResponder->onKeyEvent(inputs);
                        }
                            break;
                        default:
                            XUI::UIResponder::sFirstResponder->insertText(text);
                    }
                }
                    break;
                case AKEY_EVENT_ACTION_UP:
                    break;
                case AKEY_EVENT_ACTION_MULTIPLE: {
                    char text = AKeyEvent_getKeyCode(event);
                    XUI::UIResponder::sFirstResponder->insertText(&text);
                }
                    break;
            }
        }
            break;
    }
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
                auto androidWindow = std::dynamic_pointer_cast<XWindow_android>(
                        XDUILib::XApp::thisApp().mainWindow());
                androidWindow->init(engine);
                JNIBridgeForNativeActivity::getInstance().init(engine->app);
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
        case APP_CMD_CUSTOM: {
            doMainRunloop();
        }
            break;
    }
}


void displayKeyboard(bool pShow, android_app* mApplication) {
    // Attaches the current thread to the JVM.
    jint lResult;
    jint lFlags = 0;

    JavaVM* lJavaVM = mApplication->activity->vm;
    JNIEnv* lJNIEnv = mApplication->activity->env;

    JavaVMAttachArgs lJavaVMAttachArgs;
    lJavaVMAttachArgs.version = JNI_VERSION_1_6;
    lJavaVMAttachArgs.name = "NativeThread";
    lJavaVMAttachArgs.group = NULL;

    lResult=lJavaVM->AttachCurrentThread(&lJNIEnv, &lJavaVMAttachArgs);
    if (lResult == JNI_ERR) {
        return;
    }

    jclass ClassJNIBridge = lJNIEnv->FindClass("com/moemiku/xduilib/JNIBridgeForNativeActivity");

    // Retrieves NativeActivity.
    jobject lNativeActivity = mApplication->activity->clazz;
    jclass ClassNativeActivity = lJNIEnv->GetObjectClass(lNativeActivity);

    // Retrieves Context.INPUT_METHOD_SERVICE.
    jclass ClassContext = lJNIEnv->FindClass("android/content/Context");
    jfieldID FieldINPUT_METHOD_SERVICE =
            lJNIEnv->GetStaticFieldID(ClassContext,
                                      "INPUT_METHOD_SERVICE", "Ljava/lang/String;");
    jobject INPUT_METHOD_SERVICE =
            lJNIEnv->GetStaticObjectField(ClassContext,
                                          FieldINPUT_METHOD_SERVICE);
    //jniCheck(INPUT_METHOD_SERVICE);

    // Runs getSystemService(Context.INPUT_METHOD_SERVICE).
    jclass ClassInputMethodManager = lJNIEnv->FindClass(
            "android/view/inputmethod/InputMethodManager");
    jmethodID MethodGetSystemService = lJNIEnv->GetMethodID(
            ClassNativeActivity, "getSystemService",
            "(Ljava/lang/String;)Ljava/lang/Object;");
    jobject lInputMethodManager = lJNIEnv->CallObjectMethod(
            lNativeActivity, MethodGetSystemService,
            INPUT_METHOD_SERVICE);

    // Runs getWindow().getDecorView().
    jmethodID MethodGetWindow = lJNIEnv->GetMethodID(
            ClassNativeActivity, "getWindow",
            "()Landroid/view/Window;");
    jobject lWindow = lJNIEnv->CallObjectMethod(lNativeActivity,
                                                MethodGetWindow);
    jclass ClassWindow = lJNIEnv->FindClass(
            "android/view/Window");
    jmethodID MethodGetDecorView = lJNIEnv->GetMethodID(
            ClassWindow, "getDecorView", "()Landroid/view/View;");
    jobject lDecorView = lJNIEnv->CallObjectMethod(lWindow,
                                                   MethodGetDecorView);

    if (pShow) {
        // Runs lInputMethodManager.showSoftInput(...).
        jmethodID MethodShowSoftInput = lJNIEnv->GetMethodID(
                ClassInputMethodManager, "showSoftInput",
                "(Landroid/view/View;I)Z");
        jboolean lResult = lJNIEnv->CallBooleanMethod(
                lInputMethodManager, MethodShowSoftInput,
                lDecorView, lFlags);
    } else {
        // Runs lWindow.getViewToken()
        jclass ClassView = lJNIEnv->FindClass(
                "android/view/View");
        jmethodID MethodGetWindowToken = lJNIEnv->GetMethodID(
                ClassView, "getWindowToken", "()Landroid/os/IBinder;");
        jobject lBinder = lJNIEnv->CallObjectMethod(lDecorView,
                                                    MethodGetWindowToken);

        // lInputMethodManager.hideSoftInput(...).
        jmethodID MethodHideSoftInput = lJNIEnv->GetMethodID(
                ClassInputMethodManager, "hideSoftInputFromWindow",
                "(Landroid/os/IBinder;I)Z");
        jboolean lRes = lJNIEnv->CallBooleanMethod(
                lInputMethodManager, MethodHideSoftInput,
                lBinder, lFlags);
    }

    // Finished with the JVM.
    lJavaVM->DetachCurrentThread();
}




namespace XDUILib {

    void XApp::showSoftKeyBoard(bool bShow) {
//        if(bShow) {
//            ANativeActivity_showSoftInput(gEngine->app->activity, ANATIVEACTIVITY_SHOW_SOFT_INPUT_FORCED);
//        } else {
//            ANativeActivity_hideSoftInput(gEngine->app->activity, ANATIVEACTIVITY_SHOW_SOFT_INPUT_FORCED);
//        }
        //displayKeyboard(bShow, gEngine->app);
        JNIBridgeForNativeActivity::getInstance().showIME();
    }

    int XApp::run(struct android_app *state) {
        XApp::_thisApp = this;
        init();
        if (_mainWindow == nullptr) {
            return -1;
        }


        this->internalInit();
        initMainRunloop(state);

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
            // We are starting with a previous saved state; restore from it.chan
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
            auto androidWindow = std::dynamic_pointer_cast<XWindow_android>(
                    XDUILib::XApp::thisApp().mainWindow());
            androidWindow->update();
        }
        return 0;
    }
}