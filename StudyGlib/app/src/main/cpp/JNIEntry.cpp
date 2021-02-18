//
// Created by Administrator on 2021/2/18 0018.
//

#include "jni.h"
#include "test/person.h"
#include "core/and_log.h"

extern "C"
JNIEXPORT void JNICALL
Java_com_heaven7_android_glib_app_LibGlib_nTest(JNIEnv *env, jclass clazz, jstring name, jint val) {

    auto str = env->GetStringUTFChars(name, NULL);
    if(!test_person_get_set(str, val)){
        env->ReleaseStringUTFChars(name, str);
        return;
    }
    env->ReleaseStringUTFChars(name, str);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_heaven7_android_glib_app_LibGlib_nInit(JNIEnv *env, jclass clazz) {
    //study_person_class_init(STUDY_PERSON_GET_CLASS(study_person_parent_class));
    //extern GType study_person_get_type( void );

#ifdef HAVE_TYPE_INIT
    /* Before glib 2.36 you have to call this on startup.
	 */
	g_type_init();
#endif /*HAVE_TYPE_INIT*/

    study_person_get_type();
    LOGD("LibGlib_nInit");
}