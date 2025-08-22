package com.jin.jni;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import com.jin.jni.databinding.ActivityMainBinding;

import java.util.List;

public class MainActivity extends AppCompatActivity {


    static {
        System.loadLibrary("mainlib");
        System.loadLibrary("dynamiclib");
        System.loadLibrary("activecalllib");
        System.loadLibrary("memorylib");
    }

    private ActivityMainBinding binding;

    @SuppressLint("MissingSuperCall")
    protected native void onResume();


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        TextView tv = binding.sampleText;

        tv.setText(stringFromJNI());
        tv.setText(stringFromJNIWithDynamic1("data1"));
        tv.setText(String.valueOf(stringFromJNIWithDynamic2("data2",100)));
        tv.setText(callPathSoFunc(getSoLibraryPath(this) + "/libmainlib.so"));

        callMemoryFunc();
        createClassFromThread();
    }


    public native String stringFromJNI();

    public native String stringFromJNIWithDynamic1(String data);

    public native int stringFromJNIWithDynamic2(String data,int digit);

    public native String callPathSoFunc(String path);

    public native String callMemoryFunc();

    public native void createClassFromThread();


    public String getSoLibraryPath(Context context) {
        PackageManager packageManager = context.getPackageManager();
        List<PackageInfo> installedPackages = packageManager.getInstalledPackages(0);
        for (int i = 0; i < installedPackages.size(); i++) {
            PackageInfo packageInfo = installedPackages.get(i);
            if (packageInfo.applicationInfo.nativeLibraryDir.startsWith("/data/app")) {
                if (packageInfo.packageName.contains("com.jin")) {
                    Log.d("TAG", "getSoLibraryPath: " + packageInfo.packageName + "---->" + packageInfo.applicationInfo.nativeLibraryDir);
                    return packageInfo.applicationInfo.nativeLibraryDir;
                }
            }
        }
        return "";
    }
}