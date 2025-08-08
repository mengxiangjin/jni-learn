#include <jni.h>
#include <string>
#include <android/log.h>
#include <pthread.h>


#define TAG "xiaojianbang"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)

 void test_dynamic();
 void native_libB_test() {
    LOGD("native_libB_test");
    test_dynamic();
}

extern "C" void native_libB_testA() {
    LOGD("native_libB_testA");
}

extern "C" jstring native_libB_testB(JNIEnv  *env,char* data) {
    LOGD("native_libB_testB data is %s",data);
    return env->NewStringUTF("native_libB_testB_result");
}
