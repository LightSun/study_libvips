//
// Created by Administrator on 2021/2/18 0018.
//

#ifndef STUDYGLIB_GLIB_ALL_H
#define STUDYGLIB_GLIB_ALL_H

#include "type.h"

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

#include <glib.h>
#include <glib/gstdio.h>
#include <gmodule.h>
#include <glib-object.h>

/* Needed for VipsGInputStream.
 */
#include <gio/gio.h>

/* Handy!
 */
#ifdef VIPS_DEBUG
#define VIPS_UNREF( X ) G_STMT_START { \
	if( X ) { \
		g_assert( G_OBJECT( X )->ref_count > 0 ); \
		g_object_unref( X ); \
		(X) = 0; \
	} \
} G_STMT_END
#else /*!VIPS_DEBUG*/
#define VIPS_UNREF( X ) VIPS_FREEF( g_object_unref, (X) )
#endif /*VIPS_DEBUG*/

typedef struct _VipsObject VipsObject;
typedef struct _VipsObjectClass VipsObjectClass;

typedef void *(*VipsSListMap2Fn)( void *item,
                                  void *a, void *b );

/* Track extra stuff for arguments to objects
 */

typedef enum /*< flags >*/ {
    VIPS_ARGUMENT_NONE = 0,
    VIPS_ARGUMENT_REQUIRED = 1,
    VIPS_ARGUMENT_CONSTRUCT = 2,
    VIPS_ARGUMENT_SET_ONCE = 4,
    VIPS_ARGUMENT_SET_ALWAYS = 8,
    VIPS_ARGUMENT_INPUT = 16,
    VIPS_ARGUMENT_OUTPUT = 32,
    VIPS_ARGUMENT_DEPRECATED = 64,
    VIPS_ARGUMENT_MODIFY = 128
} VipsArgumentFlags;

/* Useful flag combinations. User-visible ones are:

VIPS_ARGUMENT_REQUIRED_INPUT 	Eg. the "left" argument for an add operation

VIPS_ARGUMENT_OPTIONAL_INPUT 	Eg. the "caption" for an object

VIPS_ARGUMENT_REQUIRED_OUTPUT  	Eg. the "result" of an add operation

VIPS_ARGUMENT_OPTIONAL_OUTPUT   Eg. the x pos of the image minimum

   Other combinations are used internally, eg. supplying the cast-table for an
   arithmetic operation

 */

#define VIPS_ARGUMENT_REQUIRED_INPUT \
	(VIPS_ARGUMENT_INPUT | \
	 VIPS_ARGUMENT_REQUIRED | \
	 VIPS_ARGUMENT_CONSTRUCT)

#define VIPS_ARGUMENT_OPTIONAL_INPUT \
	(VIPS_ARGUMENT_INPUT | \
	 VIPS_ARGUMENT_CONSTRUCT)

#define VIPS_ARGUMENT_REQUIRED_OUTPUT \
	(VIPS_ARGUMENT_OUTPUT | \
	 VIPS_ARGUMENT_REQUIRED | \
	 VIPS_ARGUMENT_CONSTRUCT)

#define VIPS_ARGUMENT_OPTIONAL_OUTPUT \
	(VIPS_ARGUMENT_OUTPUT | \
	 VIPS_ARGUMENT_CONSTRUCT)

#define VIPS_ARG_IMAGE( CLASS, NAME, PRIORITY, LONG, DESC, FLAGS, OFFSET ) { \
	GParamSpec *pspec; \
	\
	pspec = g_param_spec_object( (NAME), (LONG), (DESC),  \
		VIPS_TYPE_IMAGE, \
		(GParamFlags) (G_PARAM_READWRITE) ); \
	g_object_class_install_property( G_OBJECT_CLASS( CLASS ), \
		vips_argument_get_id(), pspec ); \
	vips_object_class_install_argument( VIPS_OBJECT_CLASS( CLASS ), \
		pspec, (VipsArgumentFlags) (FLAGS), (PRIORITY), (OFFSET) ); \
}

#define VIPS_ARG_OBJECT( CLASS, NAME, PRIORITY, LONG, DESC, FLAGS, OFFSET, TYPE ) { \
	GParamSpec *pspec; \
	\
	pspec = g_param_spec_object( (NAME), (LONG), (DESC),  \
		TYPE, \
		(GParamFlags) (G_PARAM_READWRITE) ); \
	g_object_class_install_property( G_OBJECT_CLASS( CLASS ), \
		vips_argument_get_id(), pspec ); \
	vips_object_class_install_argument( VIPS_OBJECT_CLASS( CLASS ), \
		pspec, (VipsArgumentFlags) (FLAGS), (PRIORITY), (OFFSET) ); \
}

#define VIPS_ARG_INTERPOLATE( CLASS, NAME, PRIORITY, LONG, DESC, FLAGS, OFFSET ) \
	VIPS_ARG_OBJECT( CLASS, NAME, PRIORITY, LONG, DESC, FLAGS, OFFSET, VIPS_TYPE_INTERPOLATE )

#define VIPS_ARG_BOOL( CLASS, NAME, PRIORITY, LONG, DESC, \
	FLAGS, OFFSET, VALUE ) { \
	GParamSpec *pspec; \
	\
	pspec = g_param_spec_boolean( (NAME), (LONG), (DESC), \
		(VALUE), \
		(GParamFlags) (G_PARAM_READWRITE) ); \
	g_object_class_install_property( G_OBJECT_CLASS( CLASS ), \
		vips_argument_get_id(), pspec ); \
	vips_object_class_install_argument( VIPS_OBJECT_CLASS( CLASS ), \
		pspec, (VipsArgumentFlags) (FLAGS), (PRIORITY), (OFFSET) ); \
}

#define VIPS_ARG_DOUBLE( CLASS, NAME, PRIORITY, LONG, DESC, \
	FLAGS, OFFSET, MIN, MAX, VALUE ) { \
	GParamSpec *pspec; \
	\
	pspec = g_param_spec_double( (NAME), (LONG), (DESC), \
		(MIN), (MAX), (VALUE), \
		(GParamFlags) (G_PARAM_READWRITE) );\
	g_object_class_install_property( G_OBJECT_CLASS( CLASS ), \
		vips_argument_get_id(), pspec ); \
	vips_object_class_install_argument( VIPS_OBJECT_CLASS( CLASS ), \
		pspec, (VipsArgumentFlags) (FLAGS), (PRIORITY), (OFFSET) ); \
}

#define VIPS_ARG_BOXED( CLASS, NAME, PRIORITY, LONG, DESC, \
	FLAGS, OFFSET, TYPE ) { \
	GParamSpec *pspec; \
	\
	pspec = g_param_spec_boxed( (NAME), (LONG), (DESC), \
		(TYPE), \
		(GParamFlags) (G_PARAM_READWRITE) );\
	g_object_class_install_property( G_OBJECT_CLASS( CLASS ), \
		vips_argument_get_id(), pspec ); \
	vips_object_class_install_argument( VIPS_OBJECT_CLASS( CLASS ), \
		pspec, (VipsArgumentFlags) (FLAGS), (PRIORITY), (OFFSET) ); \
}

#define VIPS_ARG_INT( CLASS, NAME, PRIORITY, LONG, DESC, \
	FLAGS, OFFSET, MIN, MAX, VALUE ) { \
	GParamSpec *pspec; \
	\
	pspec = g_param_spec_int( (NAME), (LONG), (DESC), \
		(MIN), (MAX), (VALUE), \
		(GParamFlags) (G_PARAM_READWRITE) );\
	g_object_class_install_property( G_OBJECT_CLASS( CLASS ), \
		vips_argument_get_id(), pspec ); \
	vips_object_class_install_argument( VIPS_OBJECT_CLASS( CLASS ), \
		pspec, (VipsArgumentFlags) (FLAGS), (PRIORITY), (OFFSET) ); \
}

#define VIPS_ARG_UINT64( CLASS, NAME, PRIORITY, LONG, DESC, \
	FLAGS, OFFSET, MIN, MAX, VALUE ) { \
	GParamSpec *pspec; \
	\
	pspec = g_param_spec_uint64( (NAME), (LONG), (DESC), \
		(MIN), (MAX), (VALUE), \
		(GParamFlags) (G_PARAM_READWRITE) );\
	g_object_class_install_property( G_OBJECT_CLASS( CLASS ), \
		vips_argument_get_id(), pspec ); \
	vips_object_class_install_argument( VIPS_OBJECT_CLASS( CLASS ), \
		pspec, (VipsArgumentFlags) (FLAGS), (PRIORITY), (OFFSET) ); \
}

#define VIPS_ARG_ENUM( CLASS, NAME, PRIORITY, LONG, DESC, \
	FLAGS, OFFSET, TYPE, VALUE ) { \
	GParamSpec *pspec; \
	\
	pspec = g_param_spec_enum( (NAME), (LONG), (DESC), \
		(TYPE), (VALUE), \
		(GParamFlags) (G_PARAM_READWRITE) );\
	g_object_class_install_property( G_OBJECT_CLASS( CLASS ), \
		vips_argument_get_id(), pspec ); \
	vips_object_class_install_argument( VIPS_OBJECT_CLASS( CLASS ), \
		pspec, (VipsArgumentFlags) (FLAGS), (PRIORITY), (OFFSET) ); \
}

#define VIPS_ARG_FLAGS( CLASS, NAME, PRIORITY, LONG, DESC, \
	FLAGS, OFFSET, TYPE, VALUE ) { \
	GParamSpec *pspec; \
	\
	pspec = g_param_spec_flags( (NAME), (LONG), (DESC), \
		(TYPE), (VALUE), \
		(GParamFlags) (G_PARAM_READWRITE) );\
	g_object_class_install_property( G_OBJECT_CLASS( CLASS ), \
		vips_argument_get_id(), pspec ); \
	vips_object_class_install_argument( VIPS_OBJECT_CLASS( CLASS ), \
		pspec, (VipsArgumentFlags) (FLAGS), (PRIORITY), (OFFSET) ); \
}

#define VIPS_ARG_STRING( CLASS, NAME, PRIORITY, LONG, DESC, FLAGS, OFFSET, \
	VALUE ) { \
	GParamSpec *pspec; \
	\
	pspec = g_param_spec_string( (NAME), (LONG), (DESC), \
		(VALUE), \
		(GParamFlags) (G_PARAM_READWRITE) ); \
	g_object_class_install_property( G_OBJECT_CLASS( CLASS ), \
		vips_argument_get_id(), pspec ); \
	vips_object_class_install_argument( VIPS_OBJECT_CLASS( CLASS ), \
		pspec, (VipsArgumentFlags) (FLAGS), (PRIORITY), (OFFSET) ); \
}

#define VIPS_ARG_POINTER( CLASS, NAME, PRIORITY, LONG, DESC, FLAGS, OFFSET ) { \
	GParamSpec *pspec; \
	\
	pspec = g_param_spec_pointer( (NAME), (LONG), (DESC), \
		(GParamFlags) (G_PARAM_READWRITE) ); \
	g_object_class_install_property( G_OBJECT_CLASS( CLASS ),  \
		vips_argument_get_id(), pspec ); \
	vips_object_class_install_argument( VIPS_OBJECT_CLASS( CLASS ), \
		pspec, (VipsArgumentFlags) (FLAGS), (PRIORITY), (OFFSET) ); \
}

/* Keep one of these for every argument.
 */
typedef struct _VipsArgument {
    GParamSpec *pspec;	/* pspec for this argument */

    /* More stuff, see below */
} VipsArgument;

/* Keep one of these in the class struct for every argument.
 */
typedef struct _VipsArgumentClass {
    VipsArgument parent;

    /* The class of the object we are an arg for.
     */
    VipsObjectClass *object_class;

    VipsArgumentFlags flags;
    int priority;		/* Order args by this */
    guint offset;		/* G_STRUCT_OFFSET of member in object */
} VipsArgumentClass;

/* Keep one of these in the object struct for every argument instance.
 */
typedef struct _VipsArgumentInstance {
    VipsArgument parent;

    /* The class we are part of.
     */
    VipsArgumentClass *argument_class;

    /* The object we are attached to.
     */
    VipsObject *object;

    /* Has been set.
     */
    gboolean assigned;

    /* If this is an output argument, keep the id of our "close" handler
     * here.
     */
    gulong close_id;

    /* We need to listen for "invalidate" on input images and send our own
     * "invalidate" out. If we go, we need to disconnect.
     */
    gulong invalidate_id;
} VipsArgumentInstance;


/* We have to loop over an objects args in several places, and we can't always
 * use vips_argument_map(), the preferred looper. Have the loop code as a
 * macro as well for these odd cases.
 */
#define VIPS_ARGUMENT_FOR_ALL( OBJECT, PSPEC, ARG_CLASS, ARG_INSTANCE ) { \
	VipsObjectClass *object_class = VIPS_OBJECT_GET_CLASS( OBJECT ); \
	GSList *p; \
 	\
	for( p = object_class->argument_table_traverse; p; p = p->next ) { \
		VipsArgumentClass *ARG_CLASS = \
			(VipsArgumentClass *) p->data; \
		VipsArgument *argument = (VipsArgument *) argument_class; \
		GParamSpec *PSPEC = argument->pspec; \
		VipsArgumentInstance *ARG_INSTANCE __attribute__ ((unused)) = \
			vips__argument_get_instance( argument_class, \
			VIPS_OBJECT( OBJECT ) ); \

#define VIPS_ARGUMENT_FOR_ALL_END } }

/* And some macros to collect args from a va list.
 *
 * Use something like this:

	GParamSpec *pspec;
	VipsArgumentClass *argument_class;
	VipsArgumentInstance *argument_instance;

	if( vips_object_get_argument( VIPS_OBJECT( operation ), name,
		&pspec, &argument_class, &argument_instance ) )
		return( -1 );

	VIPS_ARGUMENT_COLLECT_SET( pspec, argument_class, ap );

		GValue value holds the value of an input argument, do
		something with it

	VIPS_ARGUMENT_COLLECT_GET( pspec, argument_class, ap );

		void **arg points to where to write an output argument

	VIPS_ARGUMENT_COLLECT_END

 */
#define VIPS_ARGUMENT_COLLECT_SET( PSPEC, ARG_CLASS, AP ) \
	if( (ARG_CLASS->flags & VIPS_ARGUMENT_INPUT) ) { \
		GValue value = { 0, }; \
		gchar *error = NULL; \
 		\
		/* Input args are given inline, eg. ("factor", 12.0)  \
		 * and must be collected. \
		 */ \
		g_value_init( &value, G_PARAM_SPEC_VALUE_TYPE( PSPEC ) ); \
		G_VALUE_COLLECT( &value, AP, 0, &error ); \
		\
		/* Don't bother with the error message. \
		 */ \
		if( error ) { \
			VIPS_DEBUG_MSG( "VIPS_OBJECT_COLLECT_SET: err\n" ); \
			g_free( error ); \
		}

#define VIPS_ARGUMENT_COLLECT_GET( PSPEC, ARG_CLASS, AP ) \
		g_value_unset( &value ); \
	} \
	else if( (ARG_CLASS->flags & VIPS_ARGUMENT_OUTPUT) ) { \
		void **arg __attribute__ ((unused)); \
 		\
		/* Output args are a pointer to where to send the \
		 * result. \
		 */ \
		arg = va_arg( AP, void ** );

#define VIPS_ARGUMENT_COLLECT_END \
	}

extern GType vips_object_get_type(void);

#define VIPS_TYPE_OBJECT (vips_object_get_type())
#define VIPS_OBJECT( obj ) \
	(G_TYPE_CHECK_INSTANCE_CAST( (obj), VIPS_TYPE_OBJECT, VipsObject ))
#define VIPS_OBJECT_CLASS( klass ) \
	(G_TYPE_CHECK_CLASS_CAST( (klass), VIPS_TYPE_OBJECT, VipsObjectClass))
#define VIPS_IS_OBJECT( obj ) \
	(G_TYPE_CHECK_INSTANCE_TYPE( (obj), VIPS_TYPE_OBJECT ))
#define VIPS_IS_OBJECT_CLASS( klass ) \
	(G_TYPE_CHECK_CLASS_TYPE( (klass), VIPS_TYPE_OBJECT ))
#define VIPS_OBJECT_GET_CLASS( obj ) \
	(G_TYPE_INSTANCE_GET_CLASS( (obj), VIPS_TYPE_OBJECT, VipsObjectClass ))

/* Need to look up our VipsArgument structs from a pspec. Just hash the
* pointer (ie. we assume pspecs are never shared, is this correct?)
*/
typedef GHashTable VipsArgumentTable;

struct _VipsObject {
    GObject parent_instance;

    /* Set after ->build() has run succesfully: construct is fully done
     * and checked.
     */
    gboolean constructed;

    /* Set for static objects which are allocated at startup and never
     * freed. These objects are ommitted from leak reports.
     */
    gboolean static_object;

    /* Table of argument instances for this class and any derived classes.
     */
    VipsArgumentTable *argument_table;

    /* Class properties (see below), duplicated in the instance so we can
     * get at them easily via the property system.
     */
    char *nickname;
    char *description;

    /* The pre/post/close callbacks are all fire-once.
     */
    gboolean preclose;
    gboolean close;
    gboolean postclose;

    /* Total memory allocated relative to this object, handy for
     * profiling.
     */
    size_t local_memory;

};

/* A string in the process of being written to ... multiple calls to
 * vips_buf_append add to it. On overflow append "..." and block further
 * writes.
 */

typedef struct _VipsBuf {
    /* All fields are private.
     */
    /*< private >*/
    char *base;		/* String base */
    int mx;			/* Maximum length */
    int i;			/* Current write point */
    gboolean full;		/* String has filled, block writes */
    int lasti;		/* For read-recent */
    gboolean dynamic;	/* We own the string with malloc() */
} VipsBuf;

struct _VipsObjectClass {
    GObjectClass parent_class;

    /* Build the object ... all argument properties have been set,
     * now build the thing.
     */
    int (*build)( VipsObject *object );

    /* Just after build ... the object is fully ready for work.
     */
    int (*postbuild)( VipsObject *object, void *data );

    /* Try to print something about the class, handy for help displays.
     * Keep to one line.
     */
    void (*summary_class)( struct _VipsObjectClass *cls, VipsBuf *buf );

    /* Try to print a one-line summary for the object, the user can see
     * this output via things like "header fred.tif", --vips-cache-trace,
     * etc.
     */
    void (*summary)( VipsObject *object, VipsBuf *buf );

    /* Try to print everything about the object, handy for debugging.
     */
    void (*dump)( VipsObject *object, VipsBuf *buf );

    /* Sanity-check the object. Print messages and stuff.
     * Handy for debugging.
     */
    void (*sanity)( VipsObject *object, VipsBuf *buf );

    /* Rewind. Save and restore any stuff that needs to survive a
     * dispose().
     */
    void (*rewind)( VipsObject *object );

    /* Just before close, everything is still alive.
     */
    void (*preclose)( VipsObject *object );

    /* Close, time to free stuff.
     */
    void (*close)( VipsObject *object );

    /* Post-close, everything is dead, except the VipsObject pointer.
     * Useful for eg. deleting the file associated with a temp image.
     */
    void (*postclose)( VipsObject *object );

    /* The CLI interface. Implement these four to get CLI input and output
     * for your object.
     */

    /* Given a command-line arg (eg. a filename), make an instance of the
     * object. Just do the g_object_new(), don't call _build().
     *
     * Don't call this directly, see vips_object_new_from_string().
     */
    VipsObject *(*new_from_string)( const char *string );

    /* The inverse of ^^. Given an object, output what ->new_from_string()
     * would have been given to make that object.
     */
    void (*to_string)( VipsObject *object, VipsBuf *buf );

    /* Does this output arg need an arg from the command line? Image
     * output, for example, needs a filename to write to.
     */
    gboolean output_needs_arg;

    /* Write the object to the string. Return 0 for success, or -1 on
     * error, setting vips_error(). string is NULL if output_needs_arg()
     * was FALSE.
     */
    int (*output_to_arg)( VipsObject *object, const char *string );

    /* Class nickname, eg. "VipsInterpolateBicubic" has "bicubic" as a
     * nickname. Not internationalised.
     */
    const char *nickname;

    /* Class description. Used for help messages, so internationalised.
     */
    const char *description;

    /* Hash from pspec to VipsArgumentClass.
     *
     * This records the VipsArgumentClass for every pspec used in
     * VipsObject and any subclass (ie. everywhere), so it's huge. Don't
     * loop over this hash! Fine for lookups though.
     */
    VipsArgumentTable *argument_table;

    /* A sorted (by priority) list of the VipsArgumentClass for this class
     * and any superclasses. This is small and specific to this class.
     *
     * Use the stored GType to work out when to restart the list for a
     * subclass.
     */
    GSList *argument_table_traverse;
    GType argument_table_traverse_gtype;

    /* This class is deprecated and therefore hidden from various UI bits.
     *
     * VipsOperation has a deprecated flag, use that in preference to this
     * if you can.
     */
    gboolean deprecated;

    /* Reserved for future expansion.
     */
    void (*_vips_reserved1)( void );
    void (*_vips_reserved2)( void );
    void (*_vips_reserved3)( void );
    void (*_vips_reserved4)( void );
};

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif //STUDYGLIB_GLIB_ALL_H
