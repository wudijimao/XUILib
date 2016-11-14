package com.moemiku.xduilib;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.CompletionInfo;
import android.view.inputmethod.CorrectionInfo;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.ExtractedText;
import android.view.inputmethod.ExtractedTextRequest;
import android.view.inputmethod.InputConnection;
import android.view.inputmethod.InputMethodManager;

/**
 * Created by ximiao1 on 16/10/28.
 */

public class JNIBridgeForNativeActivity {
    class BridgeEditView extends View {

        public BridgeEditView(Context context) {
            super(context);
            mInputConnection = new BridgeInputConnection();
        }


        public class BridgeInputConnection implements InputConnection {

            public BridgeInputConnection() {
            }
            //useful
            @Override
            public boolean commitText(CharSequence text, int newCursorPosition) {
                onIMEInputStr(text.toString());
                return true;
            }

            //nouse
            @Override
            public CharSequence getTextBeforeCursor(int n, int flags) {
                return null;
            }

            @Override
            public CharSequence getTextAfterCursor(int n, int flags) {
                return null;
            }

            @Override
            public CharSequence getSelectedText(int flags) {
                return null;
            }

            @Override
            public int getCursorCapsMode(int reqModes) {
                return 0;
            }

            @Override
            public ExtractedText getExtractedText(ExtractedTextRequest request, int flags) {
                return null;
            }

            @Override
            public boolean deleteSurroundingText(int beforeLength, int afterLength) {
                return false;
            }

            @Override
            public boolean setComposingText(CharSequence text, int newCursorPosition) {
                return false;
            }

            @Override
            public boolean setComposingRegion(int start, int end) {
                return false;
            }

            @Override
            public boolean finishComposingText() {
                return false;
            }

            @Override
            public boolean commitCompletion(CompletionInfo text) {
                return false;
            }

            @Override
            public boolean commitCorrection(CorrectionInfo correctionInfo) {
                return false;
            }

            @Override
            public boolean setSelection(int start, int end) {
                return false;
            }

            @Override
            public boolean performEditorAction(int editorAction) {
                return false;
            }

            @Override
            public boolean performContextMenuAction(int id) {
                return false;
            }

            @Override
            public boolean beginBatchEdit() {
                return false;
            }

            @Override
            public boolean endBatchEdit() {
                return false;
            }

            @Override
            public boolean sendKeyEvent(KeyEvent event) {
                return false;
            }

            @Override
            public boolean clearMetaKeyStates(int states) {
                return false;
            }

            @Override
            public boolean reportFullscreenMode(boolean enabled) {
                return false;
            }

            @Override
            public boolean performPrivateCommand(String action, Bundle data) {
                return false;
            }

            @Override
            public boolean requestCursorUpdates(int cursorUpdateMode) {
                return false;
            }
        }

        InputConnection mInputConnection;

//        @Override
//        protected void onTextChanged(CharSequence text, int start, int lengthBefore, int lengthAfter) {
//            super.onTextChanged(text, start, lengthBefore, lengthAfter);
//            if (text.length() > 0) {
//                onIMEInputStr("aaa");
//            }
//        }


        @Override
        public InputConnection onCreateInputConnection(EditorInfo outAttrs) {
            return mInputConnection;
        }

        @Override
        public boolean checkInputConnectionProxy(View view) {
            return true;
        }

        @Override
        public boolean onCheckIsTextEditor() {
            return true;
        }
    }

    private BridgeEditView mBkgBridgeEditView;
    private Activity mNativeActivityInstance;

    public void init(Activity activity) {
        mNativeActivityInstance = activity;

        mBkgBridgeEditView = new BridgeEditView(activity);
        mBkgBridgeEditView.setFocusable(true);
        mBkgBridgeEditView.setFocusableInTouchMode(true);
        ((ViewGroup) mNativeActivityInstance.getWindow().getDecorView()).addView(mBkgBridgeEditView);
    }

    public void unInit() {
        mNativeActivityInstance = null;

        View view = mNativeActivityInstance.getWindow().getDecorView();
        if (view != null) {
            ((ViewGroup)view).removeView(mBkgBridgeEditView);
        }
        mBkgBridgeEditView = null;
    }

    public void showIME() {
        InputMethodManager manager = (InputMethodManager) mNativeActivityInstance.getSystemService(Activity.INPUT_METHOD_SERVICE);
        manager.showSoftInput(mBkgBridgeEditView, 0);
    }

    static {
        System.loadLibrary("xduilib");
    }

    private native void onIMEInputStr(String str);

}
