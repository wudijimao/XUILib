//
// Created by ximiao1 on 16/11/10.
//
#include "XWindow_android.hpp"
#include "XDUILib.hpp"

#include "com_moemiku_xduilib_JNIBridgeForNativeActivity.h"

#ifdef __cplusplus
extern "C" {
#endif
JNIEXPORT void JNICALL Java_com_moemiku_xduilib_JNIBridgeForNativeActivity_onIMEInputStr
        (JNIEnv *env, jobject, jstring javaString) {
    const char *nativeString = env->GetStringUTFChars(javaString, 0);

    std::string str(nativeString);
    //XDispatch::dispatchAsnyc(XDispatch::getMainQueue(), [str](){
        auto androidWindow = std::dynamic_pointer_cast<XWindow_android>(
                XDUILib::XApp::thisApp().mainWindow());
        androidWindow->insertText(str.c_str());
    //});
    // use your string

    env->ReleaseStringUTFChars(javaString, nativeString);
}
#ifdef __cplusplus
}
#endif

void keepJNIBridgeSampleInApk() {

}