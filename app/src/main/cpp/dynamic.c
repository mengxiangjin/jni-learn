//
// Created by hh on 2025/2/18.
//

//动态注册

#include <jni.h>


jint vv1(JNIEnv *env, jobject thiz,jint a, jint b) {
    return a + b;
}

jint vv2(JNIEnv *env, jobject thiz, jstring str) {
    return 100;
}

static JNINativeMethod get_methods[] = {
        {"v1", "(II)I", (void *) vv1},
        {"v2", "(Ljava/lang/String;)I", (void *) vv2},
};

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    // ----固定的开始----
    JNIEnv *env = NULL;
    if ((*vm)->GetEnv(vm, (void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }
    // ----固定的结束----


    //找到Java中的类
    jclass clazz = (*env)->FindClass(env, "com/jin/jni/Dynamic");
    //将类中方法注册到JNI中  Dynamic中有2个方法与JNI中对应，如何对应的由get_methods决定
    int res = (*env)->RegisterNatives(env, clazz, get_methods, 2);

    // ----固定的开始----
    if (res < 0) {
        return JNI_ERR;
    }

    // ----固定的结束----
    return JNI_VERSION_1_6;
}