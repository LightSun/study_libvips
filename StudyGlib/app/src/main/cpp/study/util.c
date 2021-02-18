//
// Created by Administrator on 2021/2/18 0018.
//

#include <stdlib.h>
#include "util.h"
#include "init.h"

/* Temp buffer for snprintf() layer on old systems.
 */
#define MAX_BUF (100000)

#define VIPS_MAX_ERROR (10240)
static char vips_error_text[VIPS_MAX_ERROR] = "";
static VipsBuf vips_error_buf = VIPS_BUF_STATIC( vips_error_text );
static int vips_error_freeze_count = 0;

extern const char * vips_buf_all( VipsBuf *buf );
extern void vips_buf_rewind( VipsBuf *buf );
extern gboolean vips_buf_appendf( VipsBuf *buf, const char *fmt, ... )
        __attribute__((format(printf, 2, 3)));
extern gboolean vips_buf_vappendf( VipsBuf *buf, const char *fmt, va_list ap );
extern gboolean vips_buf_appends( VipsBuf *buf, const char *str );

/* Like strncpy(), but always NULL-terminate, and don't pad with NULLs.
 * If @n is 100 and @src is more than 99 characters, 99 are copied and the
 * final byte of @dest is set to '\0'.
 */
char *
vips_strncpy( char *dest, const char *src, int n )
{
    int i;

    g_assert( n > 0 );

    for( i = 0; i < n - 1; i++ )
        if( !(dest[i] = src[i]) )
            break;
    dest[i] = '\0';

    return( dest );
}

gboolean vips_isprefix(const char *a, const char *b){
    int i;

    for( i = 0; a[i] && b[i]; i++ )
        if( a[i] != b[i] )
            return( FALSE );

    /* If there's stuff left in a but b has finished, we must have a
     * mismatch.
     */
    if( a[i] && !b[i] )
        return( FALSE );

    return( TRUE );
}

int
vips_vsnprintf( char *str, size_t size, const char *format, va_list ap )
{
#ifdef HAVE_VSNPRINTF
    return( vsnprintf( str, size, format, ap ) );
#else /*HAVE_VSNPRINTF*/
    /* Bleurg!
     */
    int n;
    static char buf[MAX_BUF];

    /* We can't return an error code, we may already have trashed the
     * stack. We must stop immediately.
     */
    if( size > MAX_BUF )
        vips_error_exit( "panic: buffer overflow "
                         "(request to write %lu bytes to buffer of %d bytes)",
                         (unsigned long) size, MAX_BUF );
    n = vsprintf( buf, format, ap );
    if( n > MAX_BUF )
        vips_error_exit( "panic: buffer overflow "
                         "(%d bytes written to buffer of %d bytes)",
                         n, MAX_BUF );

    vips_strncpy( str, buf, size );

    return( n );
#endif /*HAVE_VSNPRINTF*/
}

int
vips_snprintf( char *str, size_t size, const char *format, ... )
{
    va_list ap;
    int n;

    va_start( ap, format );
    n = vips_vsnprintf( str, size, format, ap );
    va_end( ap );

    return( n );
}
/**
 * vips_error_exit:
 * @fmt: printf()-style format string for the message
 * @...: arguments to the format string
 *
 * Sends a formatted error message to stderr, then sends the contents of the
 * error buffer, if any, then shuts down vips and terminates the program with
 * an error code.
 *
 * @fmt may be %NULL, in which case only the error buffer is printed before
 * exiting.
 *
 * See also: vips_error().
 */
void
vips_error_exit( const char *fmt, ... )
{
    if( fmt ) {
        va_list ap;

        fprintf( stderr, "%s: ", g_get_prgname() );

        va_start( ap, fmt );
        (void) vfprintf( stderr, fmt, ap );
        va_end( ap );

        fprintf( stderr, "\n" );
    }

    fprintf( stderr, "%s", vips_error_buffer() );

    //vips_shutdown();

    if(vips__fatal){
        abort();
    } else{
        exit( 1 );
    }
}
/**
 * vips_error_buffer:
 *
 * Get a pointer to the start of the error buffer as a C string.
 * The string is owned by the error system and must not be freed.
 *
 * See also: vips_error_clear().
 *
 * Returns: the error buffer as a C string which must not be freed
 */
const char *
vips_error_buffer( void )
{
    const char *msg;

    g_mutex_lock( vips__global_lock );
    msg = vips_buf_all( &vips_error_buf );
    g_mutex_unlock( vips__global_lock );

    return( msg );
}

/**
 * vips_verror:
 * @domain: the source of the error
 * @fmt: printf()-style format string for the error
 * @ap: arguments to the format string
 *
 * Append a message to the error buffer.
 *
 * See also: vips_error().
 */
void
vips_verror( const char *domain, const char *fmt, va_list ap )
{
#ifdef VIPS_DEBUG
    {
	char txt[256];
	VipsBuf buf = VIPS_BUF_STATIC( txt );
	va_list ap2;

	vips_buf_appendf( &buf, "%s: ", domain );
	va_copy( ap2, ap );
	vips_buf_vappendf( &buf, fmt, ap2 );
	vips_buf_appends( &buf, "\n" );
	VIPS_DEBUG_MSG( "vips_verror: %s", vips_buf_all( &buf ) );
}
#endif /*VIPS_DEBUG*/

    g_mutex_lock( vips__global_lock );
    g_assert( vips_error_freeze_count >= 0 );
    if( !vips_error_freeze_count ) {
        if( domain )
            vips_buf_appendf( &vips_error_buf, "%s: ", domain );
        vips_buf_vappendf( &vips_error_buf, fmt, ap );
        vips_buf_appends( &vips_error_buf, "\n" );
    }
    g_mutex_unlock( vips__global_lock );

    if( vips__fatal )
        vips_error_exit( "vips__fatal" );
}
/**
 * vips_error_freeze:
 *
 * Stop errors being logged. Use vips_error_thaw() to unfreeze. You can
 * nest freeze/thaw pairs.
 */
void
vips_error_freeze( void )
{
    g_mutex_lock( vips__global_lock );
    g_assert( vips_error_freeze_count >= 0 );
    vips_error_freeze_count += 1;
    g_mutex_unlock( vips__global_lock );
}

/**
 * vips_error_thaw:
 *
 * Reenable error logging.
 */
void
vips_error_thaw( void )
{
    g_mutex_lock( vips__global_lock );
    vips_error_freeze_count -= 1;
    g_assert( vips_error_freeze_count >= 0 );
    g_mutex_unlock( vips__global_lock );
}

/**
 * vips_error_buffer_copy:
 *
 * Return a copy of the vips error buffer, and clear it.
 *
 * Returns: a copy of the libvips error buffer
 */
char *
vips_error_buffer_copy( void )
{
    char *msg;

    g_mutex_lock( vips__global_lock );
    msg = g_strdup( vips_buf_all( &vips_error_buf ) );
    vips_error_clear();
    g_mutex_unlock( vips__global_lock );

    return( msg );
}

/**
 * vips_error_clear:
 *
 * Clear and reset the error buffer. This is typically called after presenting
 * an error to the user.
 *
 * See also: vips_error_buffer().
 */
void
vips_error_clear( void )
{
    g_mutex_lock( vips__global_lock );
    vips_buf_rewind( &vips_error_buf );
    g_mutex_unlock( vips__global_lock );
}

/**
 * vips_error:
 * @domain: the source of the error
 * @fmt: printf()-style format string for the error
 * @...: arguments to the format string
 *
 * Format the string in the style of printf() and append to the error buffer.
 *
 * See also: vips_error_system(), vips_verror().
 */
void
vips_error( const char *domain, const char *fmt, ... )
{
    va_list ap;

    va_start( ap, fmt );
    vips_verror( domain, fmt, ap );
    va_end( ap );
}
//like java string.split
/* Like strtok(). Give a string and a list of break characters. Then:
 * - skip initial break characters
 * - EOS? return NULL
 * - skip a series of non-break characters, allow `\` as a break escape
 * - write a '\0' over the next break character and return a pointer to the
 *   char after that
 *
 * The idea is that this can be used in loops as the iterator. Example:
 *
 * char *p = " 1 2 3   "; // mutable
 * char *q;
 * int i;
 * int v[...];
 *
 * for( i = 0; (q = vips_break_token( p, " " )); i++, p = q )
 * 	v[i] = atoi( p );
 *
 * will set
 * 	v[0] = 1;
 * 	v[1] = 2;
 * 	v[2] = 3;
 *
 * or with just one pointer, provided your atoi() is OK with trailing chars
 * and you know there is at least one item there
 *
 * char *p = " 1 2 3   "; // mutable
 * int i;
 * int v[...];
 *
 * for( i = 0; p; p = vips_break_token( p, " " ) )
 *   v[i] = atoi( p );
 *
 * You can use \ to escape breaks, for example:
 *
 * vips_break_token( "hello\ world", " " ) will see a single token containing
 * a space. The \ characters are squashed out.
 */
char *
vips_break_token( char *str, const char *brk )
{
    char *p;
    char *q;

    /* Is the string empty? If yes, return NULL immediately.
     */
    if( !str ||
        !*str )
        return( NULL );

    /* Skip initial break characters.
     */
    p = str + strspn( str, brk );

    /* No item?
     */
    if( !*p )
        return( NULL );

    /* We have a token ... search for the first break character after the
 * token. strcspne() allows '\' to escape breaks, see above.
     */
    p += strcspne( p, brk );

    /* Is there string left?
     */
    if( *p ) {
        /* Write in an end-of-string mark and return the start of the
         * next token.
         */
        *p++ = '\0';
        p += strspn( p, brk );
    }

    /* There may be escaped break characters in str. Loop, squashing them
     * out.
     */
    for( q = strchr( str, '\\' ); q && *q; q = strchr( q, '\\' ) ) {
        memmove( q, q + 1, strlen( q ) );

        /* If there's \\, we don't want to squash out the second \.
         */
        q += 1;
    }

    return( p );
}
/* Exactly like strcspn(), but allow \ as an escape character.
 *
 * strspne( "hello world", " " ) == 5
 * strspne( "hello\\ world", " " ) == 12
 */
static size_t
strcspne( const char *s, const char *reject )
{
    size_t skip;

    /* If \ is one of the reject chars, no need for any looping.
     */
    if( strchr( reject, '\\' ) )
        return( strcspn( s, reject ) );

    skip = 0;
    for(;;) {
        skip += strcspn( s + skip, reject );

        /* s[skip] is at the start of the string, or the end, or on a
         * break character.
         */
        if( skip == 0 ||
            !s[skip] ||
            s[skip - 1] != '\\' )
            break;

        /* So skip points at break char and we have a '\' in the char
         * before. Step over the break.
         */
        skip += 1;
    }

    return( skip );
}
const char *
vips_enum_nick( GType enm, int v )
{
    GEnumValue *value;

    if( !(value = g_enum_get_value( g_type_class_ref( enm ), v )) )
        return( "(null)" );

    return( value->value_nick );
}