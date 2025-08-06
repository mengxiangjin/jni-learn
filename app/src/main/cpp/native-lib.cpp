#include <jni.h>
#include <string>
#include <android/log.h>
#include <pthread.h>


#define TAG "xiaojianbang"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)

void thread_fun() {
    LOGD("thread_fun 进来了");
}

void thread_fun_with_vm(JavaVM *vm) {
    JNIEnv* env = nullptr;
    vm->AttachCurrentThread(&env, nullptr);
    LOGD("thread_fun_with_vm %p",env);
}

//extern "C" :保留C风格的命名方式，防止方面名称编译过后被C++添加了修饰符相关
//JNIEXPORT:导出表当中
extern "C" JNIEXPORT jstring JNICALL
Java_com_jin_jni_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject  /* this */) {
    std::string hello = "Hello from C++";
    __android_log_print(ANDROID_LOG_DEBUG,TAG,"stringFromJNI jni fmt %d %d",100,200);
    LOGE("stringFromJNI jni fmt %d",100);
    LOGD("stringFromJNI %p",env);

    //创建线程
    long pthread;
    pthread_create(&pthread, nullptr, reinterpret_cast<void *(*)(void *)>(thread_fun), nullptr);
    pthread_join(pthread, nullptr);

    return env->NewStringUTF(hello.c_str());
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = nullptr;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        LOGD("GetEnv failed");
        return -1;
    }

    LOGD("原始地址 %p",vm);
    JavaVM *newVm = nullptr;
    env->GetJavaVM(&newVm);
    LOGD("当前地址 %p",newVm);

    long pthread;
    pthread_create(&pthread, nullptr, reinterpret_cast<void *(*)(void *)>(thread_fun_with_vm), vm);
    return JNI_VERSION_1_6;
}

