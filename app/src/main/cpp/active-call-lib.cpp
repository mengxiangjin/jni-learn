#include <jni.h>
#include <string>
#include <android/log.h>
#include <pthread.h>
#include <dlfcn.h>


#define TAG "xiaojianbang"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)

extern "C" JNIEXPORT void JNICALL
Java_com_jin_jni_MainActivity_onResume(JNIEnv* env,jobject jobject1) {
    jclass cls = env->FindClass("androidx/fragment/app/FragmentActivity");
    jmethodID methodID = env->GetMethodID(cls,"onResume", "()V");
    env->CallNonvirtualVoidMethod(jobject1,cls,methodID);
}

jobject getStaticField(JNIEnv* env,jclass jclass,char* name,char *sig) {
    jfieldID pJfieldId = env->GetStaticFieldID(jclass,name,sig);
    jobject jobject = env->GetStaticObjectField(jclass,pJfieldId);
    return jobject;
}

jobject getField(JNIEnv* env,jclass jclass,char* name,char *sig,jobject instance) {
    jfieldID pJfieldId = env->GetFieldID(jclass,name,sig);
    jobject jobject = env->GetObjectField(instance,pJfieldId);
    return jobject;
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *unused) {
    JNIEnv *env = nullptr;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        LOGD("GetEnv failed");
        return -1;
    }

    //创建对象
    jclass ndkClass = env->FindClass("com/jin/jni/bean/NDKDemo");

    jmethodID jmethodId1 = env->GetMethodID(ndkClass,"<init>", "()V");
    jobject jobject1 = env->NewObject(ndkClass,jmethodId1);

    jmethodID jmethodId2 = env->GetMethodID(ndkClass,"<init>", "(Ljava/lang/String;I)V");
    jobject jobject2 = env->NewObject(ndkClass,jmethodId2,env->NewStringUTF("参数1"),10);
    LOGD("无参对象：jobject1 %p",jobject1);
    LOGD("有参对象：jobject2 %p",jobject2);

    //获取静态字段(String)
    jstring strField1 =(jstring) getStaticField(env,ndkClass,"publicStaticStringField","Ljava/lang/String;");
    const char* field1 = env->GetStringUTFChars(strField1, nullptr);
    LOGD("静态字段：field %s",field1);

    //获取对象字段(String)
    jstring strField2 =(jstring) getField(env,ndkClass,"privateStringField","Ljava/lang/String;",jobject1);
    const char* field2 = env->GetStringUTFChars(strField2, nullptr);
    LOGD("对象字段：field %s",field2);

    //设置对象字段(String)
    jstring value = env->NewStringUTF("这是新设置的字段");

    jfieldID jfieldId1 = env->GetFieldID(ndkClass,"privateStringField", "Ljava/lang/String;");
    env->SetObjectField(jobject1,jfieldId1,value);
    jstring strField3 =(jstring) getField(env,ndkClass,"privateStringField","Ljava/lang/String;",jobject1);
    const char* field3 = env->GetStringUTFChars(strField3, nullptr);
    LOGD("对象字段设置后：field %s",field3);


    //获取对象字节数组
    jbyteArray byte_array =(jbyteArray) getField(env,ndkClass,"byteArray","[B",jobject1);
    //打印字节数组
    jsize length = env->GetArrayLength(byte_array);
    jbyte* bytes = env->GetByteArrayElements(byte_array, nullptr);
    for (jsize i=0;i<length;i++) {
        LOGD("bytes值：%d",bytes[i]);
    };

    //设置对象字节数组值
    for (jsize i=0;i<length;i++) {
        bytes[i] = (char)(100-i);
    }

    env->ReleaseByteArrayElements(byte_array,bytes,0);

    for (jsize i=0;i<length;i++) {
        LOGD("bytes值：%d",bytes[i]);
    };

    //调用静态方法，空参数、空返回值
    jmethodID jmethodID = env->GetStaticMethodID(ndkClass,"publicStaticFunc","()V");
    env->CallStaticVoidMethod(ndkClass,jmethodID);

    //调用对象方法  参数（String,int） 返回值String
    jstring res1 = static_cast<jstring>(env->CallObjectMethod(jobject1, env->GetMethodID(ndkClass,
                                                                                        "privateFunc",
                                                                                        "(Ljava/lang/String;I)Ljava/lang/String;"),
                                                             env->NewStringUTF("abc"), 10));
    LOGD("CallObjectMethod ---> %s",env->GetStringUTFChars(res1, nullptr));


    jvalue args[2];
    args[0].l = env->NewStringUTF("def");
    args[1].i = 20;
    jstring res2 = static_cast<jstring>(env->CallObjectMethodA(jobject1, env->GetMethodID(ndkClass,
                                                                                          "privateFunc",
                                                                                          "(Ljava/lang/String;I)Ljava/lang/String;"),
                                                               args));
    LOGD("CallObjectMethodA ---> %s",env->GetStringUTFChars(res2, nullptr));

    //参数是数组，返回值是数组
    jobjectArray newObjectArray = env->NewObjectArray(3,env->FindClass("java/lang/String"), nullptr);
    for (int i = 0; i < env->GetArrayLength(newObjectArray); ++i) {
        //jstring item = env->NewStringUTF("ghi" + i);
        //拼接字符串
        jstring item = env->NewStringUTF(("ghi" + std::to_string(i)).c_str());
        env->SetObjectArrayElement(newObjectArray,i,item);
    }

    jintArray intArray = static_cast<jintArray>(env->CallStaticObjectMethod(ndkClass,
                                                                              env->GetStaticMethodID(
                                                                                      ndkClass,
                                                                                      "privateStaticFunc",
                                                                                      "([Ljava/lang/String;)[I"),
                                                                              newObjectArray));
    jint *p = env->GetIntArrayElements(intArray, nullptr);
    for (int i = 0; i < env->GetArrayLength(intArray); ++i) {
        LOGD("返回值 ---》%d",p[i]);
    }
    return JNI_VERSION_1_6;
}




