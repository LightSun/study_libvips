//
// Created by Administrator on 2021/2/23 0023.
//

#include "hobject.h"

G_DEFINE_TYPE2(HObject, h_object, G_TYPE_OBJECT);

static void h_object_class_lock(HObjectClass * oth){
    h_g_mutex_global_lock();
    if(oth->_mutex == NULL){
        oth->_mutex = h_g_mutex_new();
    }
    g_mutex_lock(oth->_mutex);
    h_g_mutex_global_unlock();
}
static void h_object_class_unlock(HObjectClass * oth){
    if(oth->_mutex){
        g_mutex_unlock(oth->_mutex);
    }
}
static gboolean h_object_class_tryLock(HObjectClass * oth){
    h_g_mutex_global_lock();
    if(oth->_mutex == NULL){
        oth->_mutex = h_g_mutex_new();
    }
    const gboolean result = g_mutex_trylock(oth->_mutex);
    h_g_mutex_global_unlock();
    return result;
}

static void h_object_set_property(GObject *object, guint prop_id,
                                  const GValue *value, GParamSpec *pspec ){

}

static void h_object_get_property(GObject *object, guint prop_id,
                           GValue *value, GParamSpec *pspec ) {

}
static void h_object_finalize(GObject *object){
    HObject *obj = H_OBJECT_GET_OBJECT(object);
    G_OBJECT_CLASS(h_object_parent_class)->finalize(object);
}

static void h_object_class_init(HObjectClass* clazz){
    GObjectClass *gobject_class = G_OBJECT_CLASS( clazz );
    //GInputStreamClass *istream_class = G_INPUT_STREAM_CLASS( clazz );
    gobject_class->finalize = h_object_finalize;
    gobject_class->get_property = h_object_get_property;
    gobject_class->set_property = h_object_set_property;

    clazz->lock = h_object_class_lock;
    clazz->unlock = h_object_class_unlock;
    clazz->tryLock = h_object_class_tryLock;
}
static void h_object_lock(HObject * oth){
    h_g_mutex_global_lock();
    if(oth->_mutex == NULL){
        oth->_mutex = h_g_mutex_new();
    }
    g_mutex_lock(oth->_mutex);
    h_g_mutex_global_unlock();
}
static void h_object_unlock(HObject * oth){
    if(oth->_mutex){
        g_mutex_unlock(oth->_mutex);
    }
}
static gboolean h_object_tryLock(HObject * oth){
    h_g_mutex_global_lock();
    if(oth->_mutex == NULL){
        oth->_mutex = h_g_mutex_new();
    }
    const gboolean result = g_mutex_trylock(oth->_mutex);
    h_g_mutex_global_unlock();
    return result;
}
static void h_object_init(HObject* obj){
    obj->lock = h_object_lock;
    obj->unlock = h_object_unlock;
    obj->tryLock = h_object_tryLock;
    //g_bytes_hash(obj->parent_instance.qdata);
}