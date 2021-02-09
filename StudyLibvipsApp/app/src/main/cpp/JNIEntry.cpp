//
// Created by Administrator on 2021/2/7 0007.
//

#include "jni.h"

#include <stdio.h>
#include <vips/vips.h>
#include "vips/vips8"
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

    //目前不支持stroke. 可以使用opencv来做.
    double ink_circle[3] = {255, 0, 0};//color depend on format(VipsBandFormat)

    if (!(in = vips_image_new_from_file(inPath, NULL)))
        goto error;
    size = VIPS_IMAGE_SIZEOF_IMAGE(in);
    width = vips_image_get_width(in);
    height = vips_image_get_height(in);
    bands = vips_image_get_bands(in);
    format = vips_image_get_format(in);

    w = (float)vips_image_get_width(in);
    h = (float)vips_image_get_height(in);
    LOGD("image size = %d, width = %d, height = %d, band_num = %d, bandFormat = %d, VipsInterpretation = %d",
         size, width, height, bands, format, in->Type);

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
static int clamp(int val){
    if(val < 0){
        val = 0;
    }
    if(val > 255){
        val = 255;
    }
    return val;
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_heaven7_android_libvips_app_Libvips_nReadToBuffer(JNIEnv *env, jclass clazz, jstring jstrIn,
                                                           jobject buffer) {
    auto inPath = env->GetStringUTFChars(jstrIn, NULL);
    VipsImage *in;
    in = NULL;

    if (!(in = vips_image_new_from_file(inPath, "access", VIPS_ACCESS_SEQUENTIAL, NULL))){
        env->ReleaseStringUTFChars(jstrIn, inPath);
        return false;
    }
    //vips::VImage image(in);
    //auto newImg = image.pow(1.0).cast(VIPS_FORMAT_UCHAR); //cast to RGB 888

    //java byte buffer is signed char. vips_image_get_data data is unsigned char
    auto ptrBuffer = env->GetDirectBufferAddress(buffer);
    int* pBuf = static_cast<int *>(ptrBuffer);
    //unsigned char* data = (unsigned char *) image.data();

    unsigned char* data = (unsigned char *) vips_image_get_data(in);
    int h = vips_image_get_height(in);
    int w = vips_image_get_width(in);
   //memcpy(ptrBuffer, data, w * h * 3);

   //livips offical say image format is rgb. but here is bgr. why?
    for (int ih = 0 ; ih < h ; ih ++){
        for (int iw = 0 ; iw < w ; iw ++){
            int i = ih * w + iw;
            int idx = i * 3;
            int r = data[idx];
            int g = data[idx + 1];
            int b = data[idx + 2];
            pBuf[i] = 0xff000000 | (r << 16) | (g << 8) | b;
        }
    }

    g_object_unref(in);

    env->ReleaseStringUTFChars(jstrIn, inPath);
    return true;
}

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_heaven7_android_libvips_app_Libvips_nReadToBuffer2(JNIEnv *env, jclass clazz, jstring jstrIn) {
    auto inPath = env->GetStringUTFChars(jstrIn, NULL);
    VipsImage *in;
    in = NULL;

    if (!(in = vips_image_new_from_file(inPath, "access", VIPS_ACCESS_SEQUENTIAL, NULL))){
        env->ReleaseStringUTFChars(jstrIn, inPath);
        return NULL;
    }

    unsigned char* data = (unsigned char *) vips_image_get_data(in);
    int h = vips_image_get_height(in);
    int w = vips_image_get_width(in);
    //memcpy(ptrBuffer, data, w * h * 3);
    auto buffer = env->NewByteArray(h * w * 4);

    auto byteBuf = env->GetByteArrayElements(buffer, NULL);
    int* pBuf = reinterpret_cast<int *>(byteBuf);
    for (int ih = 0 ; ih < h ; ih ++){
        for (int iw = 0 ; iw < w ; iw ++){
            int i = ih * w + iw;
            int idx = i * 3;
            int r = data[idx];
            int g = data[idx + 1];
            int b = data[idx + 2];
            pBuf[i] = 0xff000000 | (r << 16) | (g << 8) | b;
        }
    }

    g_object_unref(in);

    env->ReleaseStringUTFChars(jstrIn, inPath);
    env->ReleaseByteArrayElements(buffer, byteBuf, 0);
    return buffer;
}