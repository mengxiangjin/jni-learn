package com.jin.jni;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

import com.jin.jni.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {


    static {
        System.loadLibrary("mainlib");
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
    }

    /**
     * A native method that is implemented by the 'jni' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}