//
// Created by Administrator on 2021/2/18 0018.
//

#ifndef STUDYGLIB_UTIL_H
#define STUDYGLIB_UTIL_H

#include "glib_all.h"

#define VIPS_MAX( A, B ) ((A) > (B) ? (A) : (B))
#define VIPS_MIN( A, B ) ((A) < (B) ? (A) : (B))

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

gboolean vips_isprefix(const char *a, const char *b);

int vips_vsnprintf( char *str, size_t size, const char *format, va_list ap );
int vips_snprintf( char *str, size_t size, const char *format, ... )
__attribute__((format(printf, 3, 4)));

char *vips_strncpy( char *dest, const char *src, int n );
char *vips_break_token( char *str, const char *brk );
static size_t
strcspne( const char *s, const char *reject );
const char *
vips_enum_nick( GType enm, int v );

//----------- error ----------------
#define VIPS_BUF_STATIC( TEXT ) \
	{ &TEXT[0], sizeof( TEXT ), 0, FALSE, 0, FALSE }

void vips_error_exit( const char *fmt, ... )
__attribute__((noreturn, format(printf, 1, 2)));

const char *vips_error_buffer( void );
void
vips_error( const char *domain, const char *fmt, ... );
void
vips_error_freeze( void );
void
vips_error_thaw( void );
void
vips_error_clear( void );

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif //STUDYGLIB_UTIL_H
