package com.jin.jni;

public class Utils {

    private String name;
    private int age;

    public Utils() {

    }

    public Utils(String name) {
        this.name = name;
    }

    public Utils(String name,int age) {
        this.name = name;
        this.age = age;
    }

    public String showName() {
        return name;
    }


    static {
        //jni 需要跟cmakeList中的add_library相同
        System.loadLibrary("utils");
    }

    public static String getSign() {
        return "I am a Sign";
    }

    public static String getSign(String a) {
        return "I am a Sign" + a;
    }

    public static String getSign(String a,int b) {
        return "I am a Sign" + a + b;
    }

    public static native int v1(int a, int b);

    public static native String v2(String a);

    public static native String v3(String a);

    public static native String v4(String a,String b);

    public static native String v5(String a);

    public static native String v6(byte[] data);

    public static native String v7();

    public static native String v8();
}
