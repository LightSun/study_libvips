//
// Created by Administrator on 2021/2/8 0008.
// for multi header need VObject. we extract from VImage8.h
//

#ifndef STUDYLIBVIPSAPP_VOBJECT_H
#define STUDYLIBVIPSAPP_VOBJECT_H

#include <vips/vips.h>

namespace vips {

    enum VSteal {
        NOSTEAL = 0,
        STEAL = 1
    };

/* A smart VipsObject pointer class ... use g_object_ref()/_unref() for
 * lifetime management.
 */
    class VObject {
    private:
        // can be NULL, see eg. VObject()
        VipsObject *vobject;

    public:
        VObject(VipsObject *new_vobject, VSteal steal = STEAL) :
                vobject(new_vobject) {
            // we allow NULL init, eg. "VImage a;"
            g_assert(!new_vobject ||
                     VIPS_IS_OBJECT(new_vobject));

#ifdef VIPS_DEBUG_VERBOSE
            printf( "VObject constructor, obj = %p, steal = %d\n",
                new_vobject, steal );
            if( new_vobject ) {
                printf( "   obj " );
                vips_object_print_name( VIPS_OBJECT( new_vobject ) );
                printf( "\n" );
            }
#endif /*VIPS_DEBUG_VERBOSE*/

            if (!steal && vobject) {
#ifdef VIPS_DEBUG_VERBOSE
                printf( "   reffing object\n" );
#endif /*VIPS_DEBUG_VERBOSE*/
                g_object_ref(vobject);
            }
        }

        VObject() :
                vobject(0) {
        }

        // copy constructor
        VObject(const VObject &a) :
                vobject(a.vobject) {
            g_assert(!vobject ||
                     VIPS_IS_OBJECT(vobject));

#ifdef VIPS_DEBUG_VERBOSE
            printf( "VObject copy constructor, obj = %p\n",
                vobject );
            printf( "   reffing object\n" );
#endif /*VIPS_DEBUG_VERBOSE*/
            if (vobject)
                g_object_ref(vobject);
        }

        // assignment ... we must delete the old ref
        VObject &operator=(const VObject &a) {
#ifdef VIPS_DEBUG_VERBOSE
            printf( "VObject assignment\n" );
            printf( "   reffing %p\n", a.vobject );
            printf( "   unreffing %p\n", vobject );
#endif /*VIPS_DEBUG_VERBOSE*/

            g_assert(!vobject ||
                     VIPS_IS_OBJECT(vobject));
            g_assert(!a.vobject ||
                     VIPS_IS_OBJECT(a.vobject));

            // delete the old ref at the end ... otherwise "a = a;" could
            // unref before reffing again
            if (a.vobject)
                g_object_ref(a.vobject);
            if (vobject)
                g_object_unref(vobject);
            vobject = a.vobject;

            return (*this);
        }

        // this mustn't be virtual: we want this class to only be a pointer,
        // no vtable allowed
        ~VObject() {
#ifdef VIPS_DEBUG_VERBOSE
            printf( "VObject destructor\n" );
            printf( "   unreffing %p\n", vobject );
#endif /*VIPS_DEBUG_VERBOSE*/

            g_assert(!vobject ||
                     VIPS_IS_OBJECT(vobject));

            if (vobject)
                g_object_unref(vobject);
        }

        VipsObject *get_object() const {
            g_assert(!vobject ||
                     VIPS_IS_OBJECT(vobject));

            return (vobject);
        }

        bool is_null() const {
            return vobject == 0;
        }

    };
}
#endif //STUDYLIBVIPSAPP_VOBJECT_H
