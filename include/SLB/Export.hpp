#ifndef __SLB_EXPORT__
#define __SLB_EXPORT__

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
	#  if defined( _LIB ) || defined( USE_STATIC )
	#    define SLB_EXPORT
	#  elif defined( OKODE_SLB_LIBRARY )
	#    define SLB_EXPORT   __declspec(dllexport)
	#  else
	#    define SLB_EXPORT   __declspec(dllimport)
	#  endif /* SLB_LIBRARY */
#else
	#  define SLB_EXPORT
#endif  

#endif
