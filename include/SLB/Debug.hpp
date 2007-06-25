/*
    SLB - Simple Lua Binder
    Copyright (C) 2007 Jose L. Hidalgo Valiño (PpluX)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

	Jose L. Hidalgo (www.pplux.com)
	pplux@pplux.com
*/

#ifndef __SLB_DEBUG__
#define __SLB_DEBUG__


// you can redefine the function to be used to debug, should have a 
// printf-like interface.
#ifndef SLB_DEBUG_FUNC
	#include <cstdio>
	#define SLB_DEBUG_FUNC(...) \
	{\
		int __s = strlen(__FILE__); \
		int __offset = (__s > 18)? __s - 18: 0; \
		fprintf(stderr,"[%12s:%-4d] ", __FILE__+__offset, __LINE__);\
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
