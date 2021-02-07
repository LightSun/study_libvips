package com.heaven7.android.libvips.app;

public final class Libvips {

    static {
        System.loadLibrary("study_vips");
        Libvips.init("Libvips_java");
    }

    public static void init(String argv0){
        long start = System.currentTimeMillis();
        nInit(argv0);
        System.out.println("vips init cost: " + (System.currentTimeMillis() - start));
    }
    public static void destroy(){
        nDestroy();
    }

    public static boolean test1(String inputImage, String outImage){
        long start = System.currentTimeMillis();
        boolean result = nTest1(inputImage, outImage);
        System.out.println("vips test1 cost: " + (System.currentTimeMillis() - start));
        return result;
    }

    private static native void nInit(String argv0);
    private static native void nDestroy();
    private static native boolean nTest1(String in, String out);
}
