#include <jni.h>
#include <malloc.h>
#include "string.h"
//#include <string>
//extern "C" JNIEXPORT jstring JNICALL

//静态注册

//返回值
JNIEXPORT jint
//Java_包名_类名_方法名(JNIEnv* env,jclass clazz,参数名)  固定二个参数
Java_com_jin_jni_Utils_v1(
        JNIEnv* env,
        jclass clazz, /* this */
        jint a1,
        jint a2) {
    return a1 + a2;
}

//指针更改字符串
JNIEXPORT jstring
Java_com_jin_jni_Utils_v2(
        JNIEnv* env,
        jclass clazz, /* this */
        jstring a1) {
    //固定写法,字符指针指向字符串
    char *info = (*env) -> GetStringUTFChars(env,a1,NULL);
    //改变字符串的首位为A
    *info = 'A';
    info += 1;
    *info = 'Z';

    //需要将指针指回首位
    info -= 1;
    //字符数组转为字符串返回出去
    return (*env) -> NewStringUTF(env,info);
}

//数组更改字符串
JNIEXPORT jstring
Java_com_jin_jni_Utils_v3(
        JNIEnv* env,
        jclass clazz, /* this */
        jstring a1) {
    //固定写法,字符指针指向字符串
    char *info = (*env) -> GetStringUTFChars(env,a1,NULL);
    info[0] = 'A';
    info[1] = 'Z';
    return (*env) -> NewStringUTF(env,info);
}

int Get_String_len(char *info) {
    int count = 0;
    for (int i = 0; info[i] != '\0'; i++) {
        count+=1;
    }
    return count;
}

//拼接字符串
JNIEXPORT jstring
Java_com_jin_jni_Utils_v4(
        JNIEnv* env,
        jclass clazz, /* this */
        jstring a1,
        jstring a2) {
    char *info1 = (*env) -> GetStringUTFChars(env,a1,NULL);
    char *info2 = (*env) -> GetStringUTFChars(env,a2,NULL);
    char *info3 = (char *) malloc(strlen(info1) + strlen(info2) + 1);
    strcpy(info3,info1);
    strcat(info3,info2);
    return (*env) -> NewStringUTF(env,info3);
}

//传入的字符串转为16进制返回，不足2位补0
JNIEXPORT jstring
Java_com_jin_jni_Utils_v5(
        JNIEnv* env,
        jclass clazz, /* this */
        jstring a1) {
    char *info = (*env) -> GetStringUTFChars(env,a1,NULL);
    int size = Get_String_len(info);
    char result[size * 2];
    char *tmp = result;
    for (int i = 0; info[i] != '\0'; i++) {
        //将元素格式化输出到tmp指向的地址中
        sprintf(tmp,"%02x",info[i]);
        tmp += 2;
    }
    return (*env) -> NewStringUTF(env,result);
}


//传入的字节数组转为16进制返回，不足2位补0
JNIEXPORT jstring
Java_com_jin_jni_Utils_v6(
        JNIEnv* env,
        jclass clazz, /* this */
        jbyteArray a1) {
    char *info = (*env) -> GetByteArrayElements(env,a1,NULL);
    int size = Get_String_len(info);
    char result[size * 2];
    char *tmp = result;
    for (int i = 0; info[i] != '\0'; i++) {
        //将元素格式化输出到tmp指向的地址中
        sprintf(tmp,"%02x",info[i]);
        tmp += 2;
    }
    return (*env) -> NewStringUTF(env,result);
}


//调用Java类中的静态方法
JNIEXPORT jstring
Java_com_jin_jni_Utils_v7(
        JNIEnv* env,
        jclass clazz/* this */
        ) {
    //1.找到类名
    jclass cls = (*env)->FindClass(env,"com/jin/jni/Utils");
    //2.找到静态方法 最后一个参数 ()Ljava/lang/String;  参数+返回值类型

    //无参数 返回值String
    jmethodID mid1 = (*env)->GetStaticMethodID(env,cls,"getSign","()Ljava/lang/String;");
    //String参数 返回值String
    jmethodID mid2 = (*env)->GetStaticMethodID(env,cls,"getSign","(Ljava/lang/String;)Ljava/lang/String;");
    //String、int 参数 返回值String
    jmethodID mid3 = (*env)->GetStaticMethodID(env,cls,"getSign","(Ljava/lang/String;I)Ljava/lang/String;");

    //执行Java方法得到返回值return出去
    jstring res1 = (*env)->CallStaticObjectMethod(env,cls,mid1);
    jstring res2 = (*env)->CallStaticObjectMethod(env,cls,mid2,(*env) -> NewStringUTF(env,"你好"));
    jstring res3 = (*env)->CallStaticObjectMethod(env,cls,mid3,(*env) -> NewStringUTF(env,"你好"),18);
    return res3;
}

//调用Java类中的非静态方法
JNIEXPORT jstring
Java_com_jin_jni_Utils_v8(JNIEnv* env,jclass clazz
) {
    //1.找到类名
    jclass cls = (*env)->FindClass(env,"com/jin/jni/Utils");
    //2.找到构造方法
    jmethodID mid1 = (*env)->GetMethodID(env,cls,"<init>","()V");  //无参构造
    jmethodID mid2 = (*env)->GetMethodID(env,cls,"<init>","(Ljava/lang/String;)V"); //字符串参数构造
    jmethodID mid3 = (*env)->GetMethodID(env,cls,"<init>","(Ljava/lang/String;I)V"); //字符串+整型构造

    //3.通过构造函数创建对象
    jobject obj1 = (*env)->NewObject(env,cls,mid1);
    jobject obj2 = (*env)->NewObject(env,cls,mid2,(*env) -> NewStringUTF(env,"你好"));
    jobject obj3 = (*env)->NewObject(env,cls,mid3,(*env) -> NewStringUTF(env,"你好"),18);

    //4.先获取方法
    jmethodID showNameMethod = (*env)->GetMethodID(env,cls,"showName","()Ljava/lang/String;");
    jstring res1 = (*env)->CallObjectMethod(env,obj1,showNameMethod);
    jstring res2 = (*env)->CallObjectMethod(env,obj2,showNameMethod);
    jstring res3 = (*env)->CallObjectMethod(env,obj3,showNameMethod);
    return res2;
}

