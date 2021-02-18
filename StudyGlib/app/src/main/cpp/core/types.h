//
// Created by Administrator on 2021/2/18 0018.
//

#ifndef STUDYGLIB_TYPES_H
#define STUDYGLIB_TYPES_H


#ifdef __cplusplus
#define CPP_START extern "C" {
#define CPP_END }
#else
#define CPP_START
#define CPP_END
#endif

#ifndef size_t
#define size_t unsigned int
#endif

#define STUDY_GLIB_FREEF( F, S ) G_STMT_START { \
        if( S ) { \
                (void) F( (S) ); \
                (S) = 0; \
        } \
} G_STMT_END

CPP_START



CPP_END

#endif //STUDYGLIB_TYPES_H
