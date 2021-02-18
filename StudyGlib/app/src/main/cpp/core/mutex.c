//
// Created by Administrator on 2021/2/18 0018.
//
#include "mutex.h"

void
vips_g_mutex_free( GMutex *mutex )
{
#ifdef HAVE_MUTEX_INIT
    g_mutex_clear( mutex );
	g_free( mutex );
#else
    g_mutex_free( mutex );
#endif
}

/* Glib 2.32 revised the thread API. We need some compat functions.
 */

GMutex *
vips_g_mutex_new( void )
{
    GMutex *mutex;

#ifdef HAVE_MUTEX_INIT
    mutex = g_new( GMutex, 1 );
	g_mutex_init( mutex );
#else
    mutex = g_mutex_new();
#endif

    return( mutex );
}