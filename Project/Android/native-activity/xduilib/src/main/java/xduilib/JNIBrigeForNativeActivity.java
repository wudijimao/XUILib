package xduilib;

import android.app.Activity;

/**
 * Created by ximiao1 on 16/10/28.
 */

public class JNIBrigeForNativeActivity {
    private Activity mNativeActivityInstance;
    public void init(Activity activity) {
        mNativeActivityInstance = activity;
    }
    //private native onIMEInputStr(String str);
}
