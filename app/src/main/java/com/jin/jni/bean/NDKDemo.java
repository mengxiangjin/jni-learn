package com.jin.jni.bean;

import android.util.Log;

public class NDKDemo {
    public static String publicStaticStringField = "this is publicStaticStringField";
    public String publicStringField = "this is publicStringField";

    private static String privateStaticStringField = "this is privateStaticStringField";
    private String privateStringField = "this is privateStringField";
    private byte[] byteArray = new byte[]{1,2,3,4,5};

    public NDKDemo(){
        Log.d("xiaojianbang", "this is ReflectDemo()");
    }

    public NDKDemo(String str){
        Log.d("xiaojianbang", "this is ReflectDemo(String str)");
    }

    public NDKDemo(String str, int i){
        Log.d("xiaojianbang", i + " " + str);
        Log.d("xiaojianbang", "this is ReflectDemo(String str, int i)");
    }

    public static void publicStaticFunc(){
        Log.d("xiaojianbang", "this is publicStaticFunc");
    }

    public void publicFunc(){
        Log.d("xiaojianbang", "this is publicFunc");
    }

    private static int[] privateStaticFunc(String[] str){
        StringBuilder retval = new StringBuilder();
        for(String i : str) {
            retval.append(i);
        }
        Log.d("xiaojianbang", "this is privateStaticFunc: " + retval.toString());
        return new int[]{0,1,2,3,4,5,6,7,8,9};
    }

    private String privateFunc(String str, int i){
        Log.d("xiaojianbang", i + " this is privateFunc: " + str);
        return "this is from java";
    }

}
