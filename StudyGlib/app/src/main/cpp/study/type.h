//
// Created by Administrator on 2021/2/18 0018.
//

#ifndef STUDYGLIB_TYPE_H
#define STUDYGLIB_TYPE_H

#ifdef __cplusplus
#define CPP_START extern "C" {
#define CPP_END }
#else
#define CPP_START
#define CPP_END
#endif

CPP_START

#include <glib.h>
#include <glib/gstdio.h>
#include <gmodule.h>
#include <glib-object.h>

/* Needed for VipsGInputStream.
 */
#include <gio/gio.h>

#ifndef size_t
#define size_t unsigned int
#endif

#ifndef _
#define _(str) str
#define N_(str) str
#endif

#define VIPS_NUMBER( R ) ((int) (sizeof(R) / sizeof(R[0])))

#define VIPS_FREEF( F, S ) G_STMT_START { \
        if( S ) { \
                (void) F( (S) ); \
                (S) = 0; \
        } \
} G_STMT_END

#define VIPS_FREE( S ) VIPS_FREEF( g_free, (S) );

#define VIPS_SETSTR( S, V ) \
G_STMT_START { \
        const char *sst = (V); \
	\
        if( (S) != sst ) { \
                if( !(S) || !sst || strcmp( (S), sst ) != 0 ) { \
                        VIPS_FREE( S ); \
                        if( sst ) \
                                (S) = g_strdup( sst ); \
                } \
        } \
} G_STMT_END

/* Use in various small places where we need a mutex and it's not worth
 * making a private one.
 */
GMutex *vips__global_lock = NULL;
/* Leak check on exit.
 */
int vips__leak = 0;
/* abort() on the first warning or error.
 */
int vips__fatal = 0;
/* Count the number of threads we have active and report on leak test.
 */
int vips__n_active_threads = 0;

/* Also used for eg. vips_local() and friends.
 */
typedef int (*VipsCallbackFn)( void *a, void *b );

/* A ref-counted area of memory. Can hold arrays of things as well.
 */
typedef struct _VipsArea {
    void *data;
    size_t length;		/* 0 if not known */

    /* If this area represents an array, the number of elements in the
     * array. Equal to length / sizeof(element).
     */
    int n;

    /*< private >*/

    /* Reference count and lock.
     *
     * We could use an atomic int, but this is not a high-traffic data
     * structure, so a simple GMutex is OK.
     */
    int count;
    GMutex *lock;

    /* Things like ICC profiles need their own free functions.
     *
     * Set client to anything you like -- VipsArea doesn't use this.
     */
    VipsCallbackFn free_fn;
    void *client;

    /* If we are holding an array (for example, an array of double), the
     * GType of the elements and their size. 0 for not known.
     *
     * n is always length / sizeof_type, we keep it as a member for
     * convenience.
     */
    GType type;
    size_t sizeof_type;
} VipsArea;

typedef struct _VipsRefString {
    VipsArea area;
} VipsRefString;

VipsRefString *vips_ref_string_new( const char *str );
const char *vips_ref_string_get( VipsRefString *refstr, size_t *length );

GType vips_ref_string_get_type(void);
GType vips_save_string_get_type(void);
GType vips_blob_get_type(void);
/**
 * vips_value_get_ref_string:
 * @value: %GValue to get from
 * @length: (allow-none): return length here, optionally
 *
 * Get the C string held internally by the %GValue.
 *
 * Returns: (transfer none): The C string held by @value.
 */
const char *
vips_value_get_ref_string( const GValue *value, size_t *length )

#define VIPS_AREA( X ) ((VipsArea *) (X))
/**
 * VIPS_TYPE_REF_STRING:
 *
 * The #GType for a #VipsRefString.
 */
#define VIPS_TYPE_REF_STRING (vips_ref_string_get_type())
/**
 * VIPS_TYPE_SAVE_STRING:
 *
 * The #GType for a #VipsSaveString.
 */
#define VIPS_TYPE_SAVE_STRING (vips_save_string_get_type())
/**
 * VIPS_TYPE_BLOB:
 *
 * The %GType for a #VipsBlob.
 */
#define VIPS_TYPE_BLOB (vips_blob_get_type())

typedef struct _VipsBlob {
      VipsArea area;
} VipsBlob;

VipsBlob *vips_blob_new( VipsCallbackFn free_fn,
                         const void *data, size_t length );

VipsBlob *vips_blob_copy( const void *data, size_t length);

void *vips_value_get_blob(const GValue *value, size_t *length );
void vips_value_set_blob_free( GValue *value, void *data, size_t length );
void vips_value_set_blob( GValue *value,
                     VipsCallbackFn free_fn, const void *data, size_t length );

const void *vips_blob_get(VipsBlob *blob, size_t* length );

void vips_blob_set(VipsBlob *blob,VipsCallbackFn free_fn, const void *data, size_t length);

/**
 * VIPS_TYPE_ARRAY_DOUBLE:
 *
 * The #GType for a #VipsArrayDouble.
 */
#define VIPS_TYPE_ARRAY_DOUBLE (vips_array_double_get_type())

typedef struct _VipsArrayDouble {
    VipsArea area;
} VipsArrayDouble;

VipsArrayDouble *vips_array_double_new( const double *array, int n );
VipsArrayDouble *vips_array_double_newv( int n, ... );
double *vips_array_double_get( VipsArrayDouble *array, int *n );
GType vips_array_double_get_type(void);

#ifdef VIPS_DEBUG
#define VIPS_ARRAY_ADDR( X, I ) \
	(((I) >= 0 && (I) < VIPS_AREA( X )->n) ? \
	 (void *) ((VipsPel *) VIPS_AREA( X )->data + \
		VIPS_AREA( X )->sizeof_type * (I)) : \
	 (fprintf( stderr, \
		"VIPS_ARRAY_ADDR: index out of bounds, " \
		"file \"%s\", line %d\n" \
		"(index %d should have been within [0,%d])\n", \
		__FILE__, __LINE__, \
		(I), VIPS_AREA( X )->n ), NULL ))
#else /*!VIPS_DEBUG*/
#define VIPS_ARRAY_ADDR( X, I ) \
	((void *) \
	 ((unsigned char *) VIPS_AREA( X )->data + VIPS_AREA( X )->sizeof_type * (I)))
#endif /*VIPS_DEBUG*/

/**
 * VIPS_TYPE_ARRAY_INT:
 *
 * The #GType for a #VipsArrayInt.
 */
#define VIPS_TYPE_ARRAY_INT (vips_array_int_get_type())

typedef struct _VipsArrayInt {
    VipsArea area;
} VipsArrayInt;

VipsArrayInt *vips_array_int_new( const int *array, int n );
VipsArrayInt *vips_array_int_newv( int n, ... );
int *vips_array_int_get( VipsArrayInt *array, int *n );
GType vips_array_int_get_type(void);


void
vips_value_set_array_double( GValue *value, const double *array, int n );
void
vips_value_set_array( GValue *value, int n, GType type, size_t sizeof_type );
void *
vips_value_get_array( const GValue *value,
                      int *n, GType *type, size_t *sizeof_type );
int *
vips_value_get_array_int( const GValue *value, int *n );
double *
vips_value_get_array_double( const GValue *value, int *n );

CPP_END

#endif //STUDYGLIB_TYPE_H
