package com.heaven7.android.glib.app;

public final class LibGlib {

    static {
        //System.loadLibrary("intl");
        System.loadLibrary("study_glib");
    }

    public static void test(String fieldName, int val){
       nTest(fieldName, val);
    }
    public static void init(){
        nInit();
    }

    private static native void nInit();
    private static native void nTest(String fieldName, int val);
}
