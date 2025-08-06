package com.jin.jni;

import androidx.appcompat.app.AppCompatActivity;

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
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;
        tv.setText(String.valueOf(Utils.v1(20,10)));
        tv.setText(Utils.v2("bbbbbb"));
        tv.setText(Utils.v3("bbbbbb"));
        tv.setText(Utils.v4("aaa","bbb"));
        tv.setText(Utils.v5("这这这"));
        tv.setText(Utils.v6("这这这".getBytes()));
        tv.setText(Utils.v7());
        tv.setText(Utils.v8());

        tv.setText(String.valueOf(Dynamic.v1(10,20)));
        tv.setText(String.valueOf(Dynamic.v2("bbbbbb")));



        tv.setText(stringFromJNI());
        tv.setText(stringFromJNIWithDynamic1("data1"));
        tv.setText(String.valueOf(stringFromJNIWithDynamic2("data2",100)));
        getSoLibraryPath(this);
    }

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

    /**
     * A native method that is implemented by the 'jni' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native String stringFromJNIWithDynamic1(String data);

    public native int stringFromJNIWithDynamic2(String data,int digit);
}