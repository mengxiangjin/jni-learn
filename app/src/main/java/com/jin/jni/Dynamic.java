package com.jin.jni;

public class Dynamic {

    static {
        System.loadLibrary("dynamic");
    }

    public static native int v1(int a, int b);

    public static native int v2(String a);
}
