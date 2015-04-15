
#ifndef __ANDROID_IME_JNI_H__
#define __ANDROID_IME_JNI_H__

extern "C" {
    extern void setKeyboardStateJNI(int bOpen);
    extern void openKeyboardJNI();
    extern void closeKeyboardJNI();
    void setKeyboardType(int type);
    void setKeyboardReturnType(int type);
    void setCursorPos(int pos,const char* text);
}

#endif // __ANDROID_IME_JNI_H__
