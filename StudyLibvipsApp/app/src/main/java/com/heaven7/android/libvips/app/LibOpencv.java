package com.heaven7.android.libvips.app;

public final class LibOpencv {

    static {
        System.loadLibrary("c++_shared");
        System.loadLibrary("opencv_java4");
    }

    public static void init(){

    }
}
