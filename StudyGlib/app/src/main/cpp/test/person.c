//
// Created by Administrator on 2021/2/18 0018.
//

#include "person.h"
#include "../core/mutex.h"
#include "../core/and_log.h"

G_DEFINE_TYPE(Person, study_person, G_TYPE_OBJECT);

enum {
    PROP_0, //not use . just hold 0
    PROP_NAME,
    PROP_AGE,
    N_PROPERTIES
};
static GParamSpec *properties[N_PROPERTIES] = { NULL, };

static void
study_person_finalize(GObject *object )
{
    Person *p = STUDY_PERSON_GET_OBJECT( object );
    //do anything on free object
    if(p->name){
        g_free(p->name);
    }
    //STUDY_GLIB_FREEF(g_object_unref,  p->xxx)
    G_OBJECT_CLASS(study_person_parent_class)->finalize(object);
}

static void
test_object_set_age (Person *obj,
                     gint        foo, GParamSpec* spec)
{
    LOGD("test_object_set_age: target val = %d", foo);
    if (obj->age != foo)
    {
        obj->age = foo;

        g_assert (properties[PROP_AGE] != NULL);
        g_object_notify_by_pspec (G_OBJECT (obj), properties[PROP_AGE]);
        LOGD("called test_object_set_age()...");
    }
}

static void
study_person_get_property( GObject *object, guint prop_id,
                           GValue *value, GParamSpec *pspec )
{
    Person *gstream = STUDY_PERSON_GET_OBJECT( object );

    switch( prop_id ) {
        case PROP_NAME:
            g_value_set_string(value, gstream->name);
            break;

        case PROP_AGE:
            g_value_set_int(value, gstream->age);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
    }
}

static void
study_person_set_property( GObject *object, guint prop_id,
                           const GValue *value, GParamSpec *pspec )
{
    Person *gstream = STUDY_PERSON_GET_OBJECT( object );
    g_assert_cmpint (prop_id, !=, 0);
    g_assert_cmpint (prop_id, !=, N_PROPERTIES);
    g_assert (pspec == properties[prop_id]);

    //g_object_set_property (object, pspec->name, value);
    switch( prop_id ) {
        case PROP_NAME:
            //g_boxed_copy()
            //g_value_dup_boxed()
            if(gstream->name){
                //TODO free old
            }
            gstream->name = g_value_dup_string(value);
            break;

        case PROP_AGE:
            //gstream->age = g_value_get_int(value);
            test_object_set_age(gstream, g_value_get_int(value), pspec);
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
            return;
    }
    //g_object_notify_by_pspec(object, pspec);
}

static void study_person_class_init(PersonClass* clazz){
    GObjectClass *gobject_class = G_OBJECT_CLASS( clazz );
    //GInputStreamClass *istream_class = G_INPUT_STREAM_CLASS( clazz );
    gobject_class->finalize = study_person_finalize;
    gobject_class->get_property = study_person_get_property;
    gobject_class->set_property = study_person_set_property;

    //g_param_spec_object 参数： 字段名称， 昵称， 描述， 类型， flags
    //G_PARAM_CONSTRUCT_ONLY: 表示参数只用于构造函数， 会导致收不到g_object_set 不回调set_property
    //G_PARAM_EXPLICIT_NOTIFY: 显示通知
    properties[PROP_NAME] = g_param_spec_string("name", "Name", "the name of person",
                                                "", (GParamFlags)(G_PARAM_READWRITE));
    properties[PROP_AGE] = g_param_spec_int("age","Age", "the age of person",
                                            -1, G_MAXINT, 0,
                                            (GParamFlags)(G_PARAM_EXPLICIT_NOTIFY | G_PARAM_READWRITE));
    g_object_class_install_properties(gobject_class, N_PROPERTIES, properties);
  /*  g_object_class_install_property(gobject_class, PROP_NAME,
                                    g_param_spec_string("name", "Name", "the name of person",
                                                        "", (GParamFlags)(G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS) )
    );
    g_object_class_install_property(gobject_class, PROP_AGE,
                                    g_param_spec_int("age","Age", "the age of person",
                                                     0, 10000, 0,
                                                     (GParamFlags)(G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS) )
    );*/
    LOGD("study_person_class_init is called");
}
static void study_person_init(Person* p){
    p->name = NULL;
    p->age = -1;
}

typedef struct {
    const gchar *name;
    GParamSpec *pspec;
    gboolean    fired;
} TestNotifyClosure;

static void
on_notify (GObject           *gobject,
           GParamSpec        *pspec,
           TestNotifyClosure *clos)
{
    g_assert (clos->pspec == pspec);
    g_assert_cmpstr (clos->name, ==, pspec->name);
    clos->fired = TRUE;

    LOGD("called on_notify(): %s", clos->name);
}


gboolean test_person_get_set(const char* str, gint val){
    LOGD("test_person_get_set: prop = %s, value = %d", str, val);
    TestNotifyClosure clos;
    // Person* p = g_new(Person, 1);
  //  Person* p = g_object_new(STUDY_PERSON_TYPE, "name", "heaven7", NULL);
    Person* p = g_object_new(STUDY_PERSON_TYPE, NULL);

    g_assert (properties[PROP_NAME] != NULL);
    g_assert (properties[PROP_AGE] != NULL);

    g_signal_connect(p, "notify", G_CALLBACK (on_notify), &clos);

    GParamSpec* specc = g_object_class_find_property(G_OBJECT_CLASS(STUDY_PERSON_GET_CLASS(p)), str);
    if(specc == NULL){
        LOGW("Person >>> can't find property of %s", str);
        g_object_unref(p);
        return FALSE; //0
    }
    clos.name = str;
    clos.pspec = properties[PROP_AGE];
    g_object_set(p, str, val, NULL);

    clos.name = properties[PROP_NAME]->name;
    clos.pspec = properties[PROP_NAME];
    g_object_set(p, properties[PROP_NAME]->name, "heaven7", NULL);

    GValue age = {0};//must init {0}
    g_value_init(&age, G_TYPE_INT);
    g_value_set_int(&age, val);

    //g_object_freeze_notify(G_OBJECT(p));
   // g_object_set(p, str, val, NULL); //不生效 ， why?
    //g_object_notify(G_OBJECT(p), str);
    //g_object_thaw_notify(G_OBJECT(p));

   // g_object_freeze_notify(G_OBJECT(p));
   // g_object_set_property(G_OBJECT(p), str, &age); //
    //g_object_notify_by_pspec(G_OBJECT(p), specc);
    //g_object_notify(G_OBJECT(p), str);    //used for function callback
    //g_object_thaw_notify(G_OBJECT(p));

    g_object_get_property(G_OBJECT(p), str, &age);

    GValue pname = {0};
    g_value_init(&pname, G_TYPE_STRING);
    g_object_get_property(G_OBJECT(p), "name", &pname);
    LOGD("after c reflect: name = %s, age = %d", g_value_get_string(&pname), g_value_get_int(&age));
    g_value_unset(&age);
    g_value_unset(&pname);

    //g_free(p);
    g_object_unref(p);
    return TRUE; //1
}
/*
 #include "pm-dlist.h"

int
main (void)
{
        // GObject 库的类型管理系统的初始化
g_type_init ();

PMDList *list;
list = g_object_new (PM_TYPE_DLIST,
                     "head", NULL,
                     "tail", NULL,
                     NULL);
//可以通过g_object_get_property 或g_object_set_property 获取和设置属性
GValue val = {0};
g_value_init (val,G_TYPE_POINTER);
g_object_get_property (G_OBJECT(list), "tail", val);
g_object_set_property (G_OBJECT(list), "head-prev", val);
g_value_unset (val);
//也可以通过g_object_set 和 g_object_get 设置和获取属性，方法类似g_object_new
g_object_unref (list);
return 0;
}
*/