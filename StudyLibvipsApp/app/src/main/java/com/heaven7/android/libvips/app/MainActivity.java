package com.heaven7.android.libvips.app;

import com.heaven7.android.libvips.app.sample.TestLibvipsAc;

import java.util.List;

public class MainActivity extends AbsMainActivity {

    @Override
    protected void addDemos(List<ActivityInfo> list) {
        list.add(new ActivityInfo(TestLibvipsAc.class));
    }
}
