//
// Created by Administrator on 2021/2/18 0018.
//
#include "mutex.h"

static GMutex *_g_mutex = NULL;

void
h_g_mutex_free( GMutex *mutex )
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
h_g_mutex_new( void )
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

void h_g_mutex_init(){
    _g_mutex = h_g_mutex_new();
}

void h_g_mutex_destroy(){
    if(_g_mutex){
        GMutex * temp = _g_mutex;
        _g_mutex = NULL;
        h_g_mutex_free(temp);
    }
}

void h_g_mutex_global_lock(){
    if(_g_mutex){
        g_mutex_lock(_g_mutex);
    }
}
void h_g_mutex_global_unlock(){
    if(_g_mutex){
        g_mutex_unlock(_g_mutex);
    }
}

gboolean h_g_mutex_global_tryLock(){
    return _g_mutex ? g_mutex_trylock(_g_mutex) : FALSE;
}