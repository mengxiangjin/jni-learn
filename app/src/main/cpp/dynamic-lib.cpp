#include <jni.h>
#include <string>
#include <android/log.h>
#include <pthread.h>
#include <dlfcn.h>


#define TAG "xiaojianbang"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)


void test_dynamic() {
    LOGD("dynamic-lib test_dynamic");
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_jin_jni_MainActivity_callPathSoFunc(JNIEnv* env,jobject obj,jstring path) {
    std::string res = "callPathSoFunc ---> result";
    char* fileName = const_cast<char *>(env->GetStringUTFChars(path, nullptr));
    LOGD("callPathSoFunc %s",fileName);

    void* open = dlopen(fileName,RTLD_NOW);
    void (*def)() = nullptr;
    def = reinterpret_cast<void (*)()>(dlsym(open, "native_libB_testA"));
    if (def == nullptr) {
        LOGE("未找到native_libB_testA方法");
    } else{
        def();
    }

    // 3. 使用 dlsym 查找函数
    jstring (*native_libB_testB_ptr)(JNIEnv*, char*) =
    reinterpret_cast<jstring (*)(JNIEnv*, char*)>(dlsym(open, "native_libB_testB"));
    jstring result = native_libB_testB_ptr(env,"avaa");
    LOGD("获取到结果是%s",env->GetStringUTFChars(result, nullptr));
    return env->NewStringUTF(res.c_str());
}


jstring realFunc1(JNIEnv* env,jobject object,jstring a) {
    const char* s = env->GetStringUTFChars(a,JNI_FALSE);
    LOGD("realFunc1 ----> %s",s);
    std::string res = "realFunc1 ---> result";
    return env->NewStringUTF(res.c_str());
}

jint realFunc2(JNIEnv* env,jobject object,jstring a,jint b) {
    const char* s = env->GetStringUTFChars(a,JNI_FALSE);
    LOGD("realFunc2 ----> %s",s);
    LOGD("realFunc2 ----> %d",b);
    std::string res = "realFunc2 ---> result";
    return 20;
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = nullptr;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        LOGD("GetEnv failed");
        return -1;
    }
    jclass clazz = env->FindClass("com/jin/jni/MainActivity");
    JNINativeMethod methods[] = {
            {"stringFromJNIWithDynamic1", "(Ljava/lang/String;)Ljava/lang/String;",
            (void *)(realFunc1)},
            {
                "stringFromJNIWithDynamic2","(Ljava/lang/String;I)I",(void *)(realFunc2)
            }
    };
    env->RegisterNatives(clazz,methods,2);
    return JNI_VERSION_1_6;
}

