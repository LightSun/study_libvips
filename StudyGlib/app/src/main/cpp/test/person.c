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
};

static void
study_person_finalize(GObject *object )
{
    Person *p = STUDY_PERSON_GET_OBJECT( object );
    //do anything on free object
    //STUDY_GLIB_FREEF(g_object_unref,  p->xxx)
    G_OBJECT_CLASS(study_person_parent_class)->finalize(object);
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

    //g_object_set_property (object, pspec->name, value);
    switch( prop_id ) {
        case PROP_NAME:
            //g_boxed_copy()
            //g_value_dup_boxed()
            gstream->name = g_value_dup_string(value);
            break;

        case PROP_AGE:
            gstream->age = g_value_get_int(value);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
    }
}

static void study_person_class_init(PersonClass* clazz){
    GObjectClass *gobject_class = G_OBJECT_CLASS( clazz );
    //GInputStreamClass *istream_class = G_INPUT_STREAM_CLASS( clazz );
    gobject_class->finalize = study_person_finalize;
    gobject_class->get_property = study_person_get_property;
    gobject_class->set_property = study_person_set_property;

    //g_param_spec_object 参数： 字段名称， 昵称， 描述， 类型， flags
    g_object_class_install_property(gobject_class, PROP_NAME,
                                    g_param_spec_string("name", "Name", "the name of person",
                                                        "", (GParamFlags)(G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS) )
    );
    g_object_class_install_property(gobject_class, PROP_AGE,
                                    g_param_spec_int("age","Age", "the age of person",
                                                     0, 10000, 0,
                                                     (GParamFlags)(G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS) )
    );
    LOGD("study_person_class_init is called");
}
static void study_person_init(Person* p){

}

gboolean test_person_get_set(const char* str, gint val){
    // Person* p = g_new(Person, 1);
    Person* p = g_object_new(STUDY_PERSON_TYPE, "name", "heaven7", NULL);
    //Person* pp = p;

    guint prop = 100;
    GParamSpec **const spec = g_object_class_list_properties(G_OBJECT_CLASS(STUDY_PERSON_GET_CLASS(p)), &prop);
    if(spec){
        for (int i = 0; i < prop; ++i) {
            if(spec[i]){
                LOGD("field %s: " , spec[i]->name);
            }
        }
    } else{
        LOGD("no property for person");
    }

    if(g_object_class_find_property(G_OBJECT_CLASS(STUDY_PERSON_GET_CLASS(p)), str) == NULL){
        LOGW("Person >>> can't find property of %s", str);
        g_object_unref(p);
        return FALSE; //0
    }

    GValue age = {0};//must init {0}
    g_value_init(&age, G_TYPE_INT);
    g_value_set_int(&age, val);

    //g_object_set(p, str, age, NULL); //不生效 ， why?
    g_object_set_property(G_OBJECT(p), str, &age); //
    g_value_init(&age, G_TYPE_INT);

    //g_object_notify(G_OBJECT(p), str); //used for function callback

    g_object_get_property(G_OBJECT(p), str, &age);

    GValue pname = {0};
    g_value_init(&age, G_TYPE_STRING);
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