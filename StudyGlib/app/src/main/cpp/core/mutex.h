//
// Created by Administrator on 2021/2/18 0018.
//

#ifndef STUDYGLIB_MUTEX_H
#define STUDYGLIB_MUTEX_H

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

#include <glib.h>
#include <glib/gstdio.h>
#include <gmodule.h>
#include <glib-object.h>
#include <glib-2.0/glib-object.h>

/* Needed for VipsGInputStream.
 */
#include <gio/gio.h>

void
vips_g_mutex_free( GMutex *mutex );
GMutex *
vips_g_mutex_new( void );

#ifdef __cplusplus
}
#endif /*__cplusplus*/


#endif //STUDYGLIB_MUTEX_H
