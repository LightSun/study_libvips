/* abstract base class for all vips objects
 */

/*

    Copyright (C) 1991-2003 The National Gallery

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301  USA

 */

/*

    These files are distributed with VIPS - http://www.vips.ecs.soton.ac.uk

 */

#ifndef VIPS_OBJECT_H
#define VIPS_OBJECT_H

#include "glib_all.h"

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

int vips_argument_get_id( void );
void vips__object_set_member( VipsObject *object, GParamSpec *pspec,
	GObject **member, GObject *argument );
typedef void *(*VipsArgumentMapFn)( VipsObject *object, GParamSpec *pspec,
	VipsArgumentClass *argument_class, 
	VipsArgumentInstance *argument_instance, void *a, void *b );
void *vips_argument_map( VipsObject *object, 
	VipsArgumentMapFn fn, void *a, void *b );
int vips_object_get_args( VipsObject *object, 
	const char ***names, int **flags, int *n_args );
typedef void *(*VipsArgumentClassMapFn)( VipsObjectClass *object_class, 
	GParamSpec *pspec,
	VipsArgumentClass *argument_class, void *a, void *b );
void *vips_argument_class_map( VipsObjectClass *object_class, 
	VipsArgumentClassMapFn fn, void *a, void *b );
gboolean vips_argument_class_needsstring( VipsArgumentClass *argument_class );
int vips_object_get_argument( VipsObject *object, const char *name,
	GParamSpec **pspec,
	VipsArgumentClass **argument_class,
	VipsArgumentInstance **argument_instance );
gboolean vips_object_argument_isset( VipsObject *object, const char *name );
VipsArgumentFlags vips_object_get_argument_flags( VipsObject *object, 
	const char *name );
int vips_object_get_argument_priority( VipsObject *object, const char *name );

gboolean vips_value_is_null( GParamSpec *psoec, const GValue *value );
void vips_object_set_property( GObject *gobject, 
	guint property_id, const GValue *value, GParamSpec *pspec );
void vips_object_get_property( GObject *gobject, 
	guint property_id, GValue *value, GParamSpec *pspec );

void vips_object_preclose( VipsObject *object );
int vips_object_build( VipsObject *object );

void vips_object_summary_class( VipsObjectClass *klass, VipsBuf *buf );
void vips_object_summary( VipsObject *object, VipsBuf *buf );
void vips_object_dump( VipsObject *object, VipsBuf *buf );

void vips_object_print_summary_class( VipsObjectClass *klass );
void vips_object_print_summary( VipsObject *object );
void vips_object_print_dump( VipsObject *object );
void vips_object_print_name( VipsObject *object );

gboolean vips_object_sanity( VipsObject *object );

/* Don't put spaces around void here, it breaks gtk-doc.
 */
GType vips_object_get_type(void);

void vips_object_class_install_argument( VipsObjectClass *cls, 
	GParamSpec *pspec, VipsArgumentFlags flags, 
	int priority, guint offset );
int vips_object_set_argument_from_string( VipsObject *object, 
	const char *name, const char *value );
gboolean vips_object_argument_needsstring( VipsObject *object, 
	const char *name );
int vips_object_get_argument_to_string( VipsObject *object, 
	const char *name, const char *arg );
int vips_object_set_required( VipsObject *object, const char *value );

typedef void *(*VipsObjectSetArguments)( VipsObject *object, void *a, void *b );
VipsObject *vips_object_new( GType type, 
	VipsObjectSetArguments set, void *a, void *b );

int vips_object_set_valist( VipsObject *object, va_list ap );
int vips_object_set( VipsObject *object, ... )
	__attribute__((sentinel));
int vips_object_set_from_string( VipsObject *object, const char *string );

VipsObject *vips_object_new_from_string( VipsObjectClass *object_class, 
	const char *p );
void vips_object_to_string( VipsObject *object, VipsBuf *buf );

void *vips_object_map( VipsSListMap2Fn fn, void *a, void *b );

typedef void *(*VipsTypeMapFn)( GType type, void *a );
typedef void *(*VipsTypeMap2Fn)( GType type, void *a, void *b );
typedef void *(*VipsClassMapFn)( VipsObjectClass *cls, void *a );
void *vips_type_map( GType base, VipsTypeMap2Fn fn, void *a, void *b );
void *vips_type_map_all( GType base, VipsTypeMapFn fn, void *a );
int vips_type_depth( GType type );
GType vips_type_find( const char *basename, const char *nickname );
const char *vips_nickname_find( GType type );

void *vips_class_map_all( GType type, VipsClassMapFn fn, void *a );
const VipsObjectClass *vips_class_find( const char *basename, 
	const char *nickname );

VipsObject **vips_object_local_array( VipsObject *parent, int n );

void vips_object_local_cb( VipsObject *vobject, GObject *gobject );
#define vips_object_local( V, G ) \
	(g_signal_connect( V, "close", G_CALLBACK( vips_object_local_cb ), G ))

void vips_object_set_static( VipsObject *object, gboolean static_object );
void vips_object_print_all( void );
void vips_object_sanity_all( void );

void vips_object_rewind( VipsObject *object );

void vips_object_unref_outputs( VipsObject *object );

const char *vips_object_get_description( VipsObject *object );

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*VIPS_OBJECT_H*/


