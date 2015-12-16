#include <jni.h>

#ifndef _Included_mHelp_JmHelp
#define _Included_mHelp_JmHelp

#ifdef __cplusplus
extern "C" {
#endif

////// Main
JNIEXPORT void JNICALL Java_mHelp_JmHelp_Init(JNIEnv *, jobject, jlong, jstring, jboolean);
JNIEXPORT void JNICALL Java_mHelp_JmHelp_Open(JNIEnv *, jobject);

////// Extended

//
// TODO: Add extended mHelp functions here
//

#ifdef __cplusplus
}
#endif
#endif
