//
// Created by Administrator on 2020/7/7 0007.
//

#ifndef FFMPEGOPENGLDEMO_LOG_H
#define FFMPEGOPENGLDEMO_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>

#define TAG "study_vips"

#ifdef ANDROID
    #include "android/log.h"
    #define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__)
    #define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
    #define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
    #define LOGW(...) __android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__)
    #define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
    #define DBG(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
#else
    //empty. later will dosomething
    #define LOGV(...) printf(__VA_ARGS__)
    #define LOGD(...) printf(__VA_ARGS__)
    #define LOGI(...) printf(__VA_ARGS__)
    #define LOGW(...) printf(__VA_ARGS__)
    #define LOGE(...) printf(__VA_ARGS__)
    #define DBG(...) printf(__VA_ARGS__)
#endif


#ifdef __cplusplus
}
#endif

#endif //FFMPEGOPENGLDEMO_LOG_H
