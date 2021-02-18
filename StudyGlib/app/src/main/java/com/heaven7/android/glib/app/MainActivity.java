package com.heaven7.android.glib.app;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;

public class MainActivity extends AppCompatActivity {

    static {
        LibGlib.init();
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    public void onClickTestGlibBase(View view) {
       LibGlib.test("age", 19);
    }

    public void onClickTestNoProperty(View view) {
        LibGlib.test("age1", 19);
    }
}
