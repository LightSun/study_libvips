cmake_minimum_required(VERSION 3.4.1)

#asm 用clang编译。  nasm 用yasm编译
#enable_language(ASM)
enable_language(CXX)
enable_language(C)

set(LIB_DIR ${CMAKE_SOURCE_DIR}/../../jniLibs/${ANDROID_ABI})
include_directories(src/main/cpp/include)
include_directories(src/main/cpp/include/glib-2.0)
include_directories(src/main/cpp/include/glib-2.0/include)
include_directories(src/main/cpp/study)

#aux_source_directory(src/main/cpp/study SRC_CORE)
aux_source_directory(src/main/cpp/core SRC_CORE)
aux_source_directory(src/main/cpp/test SRC_CORE)

add_library(expat STATIC IMPORTED)
set_target_properties(expat PROPERTIES IMPORTED_LOCATION ${LIB_DIR}/libexpat.a)

add_library(ffi STATIC IMPORTED)
set_target_properties(ffi PROPERTIES IMPORTED_LOCATION ${LIB_DIR}/libffi.a)

add_library(gio STATIC IMPORTED)
set_target_properties(gio PROPERTIES IMPORTED_LOCATION ${LIB_DIR}/libgio-2.0.a)

add_library(glib STATIC IMPORTED)
set_target_properties(glib PROPERTIES IMPORTED_LOCATION ${LIB_DIR}/libglib-2.0.a)

add_library(gmodule STATIC IMPORTED)
set_target_properties(gmodule PROPERTIES IMPORTED_LOCATION ${LIB_DIR}/libgmodule-2.0.a)

add_library(gobject STATIC IMPORTED)
set_target_properties(gobject PROPERTIES IMPORTED_LOCATION ${LIB_DIR}/libgobject-2.0.a)

add_library(
        study_glib
        SHARED
        ${SRC_CORE}
        src/main/cpp/JNIEntry.cpp
)
find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

target_link_libraries(
        study_glib
        #m  #math
        gio
        gmodule
        gobject
        ffi
        #expat
        glib
        #z  #zlib
        #dl #dlopen/dlclose
        ${log-lib}
)
#--------------------------------------------------------------
