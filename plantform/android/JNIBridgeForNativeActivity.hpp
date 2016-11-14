/**
 * Created by ximiao1 on 16/11/10.
 */

#include "XAndroidEnv.hpp"

class JNIBridgeForNativeActivity {
public:
    static JNIBridgeForNativeActivity& getInstance() {
        static JNIBridgeForNativeActivity sBridgeInstance;
        return sBridgeInstance;
    }
    bool init(android_app* state){
        mAppState = state;
        mVm = state->activity->vm;
        // Attaches the current thread to the JVM.
        bool ret = true;
        JNIEnv* env = attachEnv();
        jobject thiz = state->activity->clazz;
        jclass classXActivity = env->GetObjectClass(thiz);
        jmethodID funGetBridge = env->GetStaticMethodID(classXActivity, "getBridge", "()Lcom/moemiku/xduilib/JNIBridgeForNativeActivity;");
        if(funGetBridge != nullptr) {
            mBridgeJavaObj = env->CallStaticObjectMethod(classXActivity, funGetBridge);
            mBridgeJavaObj = env->NewGlobalRef(mBridgeJavaObj);

            jclass classBridge = env->GetObjectClass(mBridgeJavaObj);
            mFuncShowIME = env->GetMethodID(classBridge, "showIME", "()V");
        } else {
            ret = false;
        }
        detachEnv();
        if (!ret) {
            mAppState = nullptr;

            //加入析构函数或unInit
            if(mBridgeJavaObj) {
                env->DeleteGlobalRef(mBridgeJavaObj);
            }
        };
        return ret;
    }
    void showIME() {
        callVoidMethod(mFuncShowIME);
    }
private:
    JNIEnv* attachEnv() {
        JNIEnv* env;
        mVm->AttachCurrentThread(&env, NULL);
        return env;
    }
    void detachEnv() {
        mVm->DetachCurrentThread();
    }
    void callVoidMethod(jmethodID id) {
        JNIEnv* env = attachEnv();
        env->CallVoidMethod(mBridgeJavaObj, id);
        detachEnv();
    }

    JavaVM* mVm = nullptr;
    android_app* mAppState = nullptr;

    jobject mBridgeJavaObj = 0;
    jmethodID mFuncSetIMECallBack = 0;
    jmethodID mFuncShowIME = 0;
};