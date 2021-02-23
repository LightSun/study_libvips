//
// Created by Administrator on 2021/2/23 0023.
//

#ifndef STUDYGLIB_HOBJECT_H
#define STUDYGLIB_HOBJECT_H


#include "../core/mutex.h"
#include "../core/types.h"

CPP_START

typedef struct _HObject HObject;
typedef struct _HObjectClass HObjectClass;

struct _HObject{
    GObject parent_instance;

    GMutex* _mutex;

    int (*hash)();
    int (*equals)(const HObject *cur,const HObject * oth);
    //like java's synchronized. lock use object's mutex
    void (*lock)(HObject * cur);
    void (*unlock)(HObject * cur);
    gboolean (*tryLock)(HObject * cur);
};

struct _HObjectClass{
    GObjectClass parent_class;

    //lock use class's mutex
    GMutex* _mutex;
    void (*lock)(HObjectClass* clazz);
    void (*unlock)(HObjectClass* clazz);
	gboolean (*tryLock)(HObjectClass * clazz);
};

#define H_OBJECT_TYPE (h_object_get_type())

#define H_OBJECT_GET_CLASS( obj ) \
	(G_TYPE_INSTANCE_GET_CLASS( (obj), H_OBJECT_TYPE, HObjectClass ))
#define H_OBJECT_GET_OBJECT( obj ) \
	(G_TYPE_CHECK_INSTANCE_CAST( (obj), \
	H_OBJECT_TYPE, HObject ))
#define H_OBJECT_CLASS( klass ) \
	(G_TYPE_CHECK_CLASS_CAST( (klass), \
	H_OBJECT_TYPE, HObjectClass))

#define H_IS_OBJECT( obj ) \
(G_TYPE_CHECK_INSTANCE_TYPE( (obj), H_OBJECT_TYPE ))
#define H_IS_OBJECT_CLASS( klass ) \
	(G_TYPE_CHECK_CLASS_TYPE( (klass), H_OBJECT_TYPE ))

GType h_object_get_type(void);
void h_object_init(HObject* obj);

#define G_DEFINE_TYPE2(TN, t_n, T_P) \
G_DEFINE_TYPE(TN, t_n, T_P) \
static void t_n##_set_property(GObject *object, guint prop_id,const GValue *value, GParamSpec *pspec);\
static void t_n##_get_property( GObject *object, guint prop_id, GValue *value, GParamSpec *pspec);

CPP_END
#endif //STUDYGLIB_HOBJECT_H
