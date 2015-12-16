#include "JmHelp.h"

#include <cstring>
#include <windows.h>
#include <mHelp.h>

HWND hwndJava = NULL;

////// Main
JNIEXPORT void JNICALL Java_mHelp_JmHelp_Init(JNIEnv *env, jobject obj, jlong hwnd, jstring ExeName, jboolean Shared) {

    hwndJava = (HWND)hwnd;
    const char *pExeName = env->GetStringUTFChars(ExeName, 0);
    mHelpInit(hwndJava, pExeName, (Shared)? true:false);
    env->ReleaseStringUTFChars(ExeName, pExeName);
}

JNIEXPORT void JNICALL Java_mHelp_JmHelp_Open(JNIEnv *env, jobject obj) {

    mHelpOpen(hwndJava);
}

////// Extended

//
// TODO: Add extended mHelp functions here
//
