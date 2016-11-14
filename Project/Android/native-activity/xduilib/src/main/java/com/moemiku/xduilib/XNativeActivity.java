package com.moemiku.xduilib;

import android.app.NativeActivity;
import android.os.Bundle;
import android.util.Log;

/**
 * Created by ximiao1 on 16/11/7.
 */

public class XNativeActivity extends NativeActivity {
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mBridge.init(this);
        Log.v("test","oncreate");
    }

    static JNIBridgeForNativeActivity mBridge = new JNIBridgeForNativeActivity();
    static JNIBridgeForNativeActivity getBridge() {
        return mBridge;
    }
}
