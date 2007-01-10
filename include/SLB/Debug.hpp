#ifndef __SLB_DEBUG__
#define __SLB_DEBUG__


// you can redefine the function to be used to debug, should have a 
// printf-like interface.
#ifndef SLB_DEBUG_FUNC
	#include <cstdio>
	#define SLB_DEBUG_FUNC(...) \
	{\
		fprintf(stderr,"[%s:%-4d] ", __FILE__, __LINE__);\
		fprintf(stderr,__VA_ARGS__);\
		fprintf(stderr,"\n");\
	}
#endif

//----------------------------------------------------------------------------
//-- Simple Debug (dissabled by default) -------------------------------------
//----------------------------------------------------------------------------
#ifndef SLB_DEBUG_LEVEL
#define SLB_DEBUG_LEVEL 0
#endif
	
#if SLB_DEBUG_LEVEL != 0
	#include<SLB/SPP.hpp>
	#define SLB_DEBUG(level,...) if (level <= SLB_DEBUG_LEVEL)\
		{\
			SLB_DEBUG_FUNC("SLB-" SPP_TOSTRING(level) " "__VA_ARGS__);\
		}
#else
	#define SLB_DEBUG(level,...)
#endif

#endif
