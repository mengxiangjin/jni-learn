#include <jni.h>
#include <string>
#include <android/log.h>
#include <pthread.h>
#include <dlfcn.h>


#define TAG "xiaojianbang"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)



extern "C" void _init() {
    LOGE("memory-lib init");
}

__attribute__((constructor)) void fun1() {
    LOGE("memory-lib fun1");
}

__attribute__((constructor)) void fun2() {
    LOGE("memory-lib fun2");
}

jclass ndkClass;
JavaVM *javaVM;

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *unused) {
    javaVM = vm;
    JNIEnv *env = nullptr;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        LOGD("GetEnv failed");
        return -1;
    }

    jclass localRef = env->FindClass("com/jin/jni/bean/NDKDemo");
    //赋值全局变量
    ndkClass = (jclass)env->NewGlobalRef(localRef);
    return JNI_VERSION_1_6;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_jin_jni_MainActivity_callMemoryFunc(JNIEnv *env,jobject obj) {
    jmethodID methodId = env->GetStaticMethodID(static_cast<jclass>(ndkClass), "publicStaticFunc", "()V");
    env->CallStaticVoidMethod(static_cast<jclass>(ndkClass), methodId);
    return env->NewStringUTF("res");
}

void create_class() {
    JNIEnv *env = nullptr;
    bool needDetach = false;
    if (javaVM->GetEnv((void **) &env, JNI_VERSION_1_4) < 0) {
        LOGD("----AttachCurrentThread----");
        javaVM->AttachCurrentThread(&env, NULL);
        needDetach = true;
    }

    jfieldID field = env->GetStaticFieldID(ndkClass, "publicStaticStringField", "Ljava/lang/String;");
    jstring data =(jstring) env->GetStaticObjectField(ndkClass, field);
    LOGD("data in threadFunc : %s", env->GetStringUTFChars(data, nullptr));
    env->ExceptionClear();
    if (needDetach) {
        LOGD("----DetachCurrentThread----");
        javaVM->DetachCurrentThread();
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_jin_jni_MainActivity_createClassFromThread(JNIEnv *env,jobject obj) {
    long pthread;
    pthread_create(&pthread, nullptr, reinterpret_cast<void *(*)(void *)>(create_class), nullptr);
}




