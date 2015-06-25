
#ifndef NC_WRAPPER_EXPORT_H
#define NC_WRAPPER_EXPORT_H

#ifdef NC_WRAPPER_BUILT_AS_STATIC
#  define NC_WRAPPER_EXPORT
#  define NC_WRAPPER_NO_EXPORT
#else
#  ifndef NC_WRAPPER_EXPORT
#    ifdef NC_WRAPPER_EXPORTS
        /* We are building this library */
#      define NC_WRAPPER_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define NC_WRAPPER_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef NC_WRAPPER_NO_EXPORT
#    define NC_WRAPPER_NO_EXPORT 
#  endif
#endif

#ifndef NC_WRAPPER_DEPRECATED
#  define NC_WRAPPER_DEPRECATED __attribute__ ((__deprecated__))
#  define NC_WRAPPER_DEPRECATED_EXPORT NC_WRAPPER_EXPORT __attribute__ ((__deprecated__))
#  define NC_WRAPPER_DEPRECATED_NO_EXPORT NC_WRAPPER_NO_EXPORT __attribute__ ((__deprecated__))
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define NC_WRAPPER_NO_DEPRECATED
#endif

#endif
