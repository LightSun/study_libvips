//
// Created by Administrator on 2021/2/7 0007.
//

#include "jni.h"

#include <stdio.h>
#include <vips/vips.h>
#include "and_log.h"

//com.heaven7.android.libvips.app
extern "C"
JNIEXPORT void JNICALL
Java_com_heaven7_android_libvips_app_Libvips_nInit(JNIEnv *env, jclass clazz, jstring arg0) {

    auto str = env->GetStringUTFChars(arg0, NULL);
    VIPS_INIT(str);
    env->ReleaseStringUTFChars(arg0, str);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_heaven7_android_libvips_app_Libvips_nDestroy(JNIEnv *env, jclass clazz) {

    vips_shutdown();
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_heaven7_android_libvips_app_Libvips_nTest1(JNIEnv *env, jclass clazz, jstring jstrIn,
                                                    jstring jstrOut) {
    auto inPath = env->GetStringUTFChars(jstrIn, NULL);
    auto outPath = env->GetStringUTFChars(jstrOut, NULL);

    VipsImage *in;
    VipsImage *out;
    float w;
    float h;

    size_t size;
    int width;
    int height;
    int bands;
    VipsBandFormat format;

    in = NULL;
    out = NULL;

    //目前不支持stroke
    double ink_circle[3] = {255, 0, 0};//color depend on format(VipsBandFormat)

    if (!(in = vips_image_new_from_file(inPath, NULL)))
        goto error;
    size = VIPS_IMAGE_SIZEOF_IMAGE(in);
    width = in->Xsize;
    height = in->Ysize;
    bands = in->Bands;
    format = in->BandFmt;

    w = (float)vips_image_get_width(in);
    h = (float)vips_image_get_height(in);
    LOGD("image width = %d\n", vips_image_get_width(in));

    // vips_autorot()
    // autorotates based on exif tag :)

    if (vips_extract_area(in, &out, (int)(0.25 * w), (int)(0.25 * h),
                          (int)(0.5 * w), (int)(0.5 * h), NULL)) {
        goto error;
    }
    //vips_draw_flood()
    if (vips_resize(out, &out, 0.5f, NULL)) {
        goto error;
    }
    /* if(vips_round(out, &out, VipsOperationRound::VIPS_OPERATION_ROUND_CEIL, NULL)){
        goto error;
    }*/
    /* if(vips_draw_circle1(out, 0.5, 300, 300, 500, NULL)){
        goto error;
    }*/
    if(vips_draw_circle(out, ink_circle, 3, 300, 300, 500, NULL)){
        goto error;
    }

    if (vips_image_write_to_file(out, outPath, NULL))
        goto error;

    g_object_unref(in);
    g_object_unref(out);
    env->ReleaseStringUTFChars(jstrIn, inPath);
    env->ReleaseStringUTFChars(jstrOut, outPath);

    return true;
    error:
    if(in != NULL){
        g_object_unref(in);
    }
    env->ReleaseStringUTFChars(jstrIn, inPath);
    env->ReleaseStringUTFChars(jstrOut, outPath);
    //vips_error_exit(NULL);
    return false;
}