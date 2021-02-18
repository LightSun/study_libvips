//
// Created by Administrator on 2021/2/18 0018.
//

#ifndef STUDYGLIB_PERSON_H
#define STUDYGLIB_PERSON_H

#include "../core/mutex.h"
#include "../core/types.h"

CPP_START

typedef struct _Person{
    GObject parent_instance;
    gint age;
    const gchar* name;
}Person;

typedef struct _PersonClass{
    GObjectClass parent_class;

    //void (*sInit)();
    //void (*sDestroy)();
}PersonClass;

#define STUDY_PERSON_TYPE (study_person_get_type())
GType study_person_get_type(void);

#define STUDY_PERSON_GET_CLASS( obj ) \
	(G_TYPE_INSTANCE_GET_CLASS( (obj), STUDY_PERSON_TYPE, PersonClass ))
#define STUDY_PERSON_GET_OBJECT( obj ) \
	(G_TYPE_CHECK_INSTANCE_CAST( (obj), \
	STUDY_PERSON_TYPE, Person ))

	//test
gboolean test_person_get_set(const char* str, int val);

CPP_END

#endif //STUDYGLIB_PERSON_H
