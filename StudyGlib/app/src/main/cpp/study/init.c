//
// Created by Administrator on 2021/2/18 0018.
//

#include "init.h"

static gboolean
vips_set_fatal_cb( const gchar *option_name, const gchar *value,
                   gpointer data, GError **error );
static gboolean
vips_lib_info_cb( const gchar *option_name, const gchar *value,
                  gpointer data, GError **error );

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

void study_glib_init(){
    if( !vips__global_lock )
        vips__global_lock = vips_g_mutex_new();
}

void study_glib_destroy(){
//todo
}

static void
vips_verbose( void )
{
    const char *old;

    old = g_getenv( "G_MESSAGES_DEBUG" );

    if( !old )
        g_setenv( "G_MESSAGES_DEBUG", G_LOG_DOMAIN, TRUE );
    else if( !g_str_equal( old, "all" ) &&
             !g_strrstr( old, G_LOG_DOMAIN ) ) {
        char *new;

        new = g_strconcat( old, " ", G_LOG_DOMAIN, NULL );
        g_setenv( "G_MESSAGES_DEBUG", new, TRUE );

        g_free( new );
    }
}
static gboolean
vips_lib_info_cb( const gchar *option_name, const gchar *value,
                  gpointer data, GError **error )
{
    vips_verbose();

    return( TRUE );
}

static gboolean
vips_set_fatal_cb( const gchar *option_name, const gchar *value,
                   gpointer data, GError **error )
{
    vips__fatal = 1;

    /* Set masks for debugging ... stop on any problem.
     */
    g_log_set_always_fatal(
            G_LOG_FLAG_RECURSION |
            G_LOG_FLAG_FATAL |
            G_LOG_LEVEL_ERROR |
            G_LOG_LEVEL_CRITICAL |
            G_LOG_LEVEL_WARNING );

    return( TRUE );
}