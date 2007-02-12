#ifndef __SLB_EXPORT__
#define __SLB_EXPORT__

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
	#  if defined( _LIB ) || defined( USE_STATIC )
	#    define SLB_EXPORT
	#  elif defined( SLB_LIBRARY )
	#    define SLB_EXPORT   __declspec(dllexport)

	#  else
	#    define SLB_EXPORT   __declspec(dllimport)
	#  endif /* SLB_LIBRARY */
	#pragma warning( disable: 4251 )	
#else
	#  define SLB_EXPORT
#endif  

#endif
