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

#ifndef SLB_DEBUG_OUTPUT
	#define SLB_DEBUG_OUTPUT stderr
#endif

// you can redefine the function to be used to debug, should have a 
// printf-like interface.
#ifndef SLB_DEBUG_FUNC
	#include <cstdio>
	#define SLB_DEBUG_FUNC(...) \
	{\
		int __s = strlen(__FILE__); \
		int __offset = (__s > 18)? __s - 18: 0; \
		fprintf(SLB_DEBUG_OUTPUT,"[%12s:%-4d] ", __FILE__+__offset, __LINE__);\
		fprintf(SLB_DEBUG_OUTPUT,__VA_ARGS__);\
		fprintf(SLB_DEBUG_OUTPUT,"\n");\
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

	#define SLB_DEBUG_STACK(level, L,  ... ) \
		{\
			SLB_DEBUG(level, " {stack} "  __VA_ARGS__ );\
			int top = lua_gettop(L); \
			for(int i = 1; i <= top; i++) \
			{ \
				if (lua_type(L,i) == LUA_TNONE) \
				{ \
					SLB_DEBUG(level, "\targ %d = (Invalid)", i);\
				} \
				else \
				{ \
					lua_pushvalue(L,i);\
					SLB_DEBUG(level, "\targ %d = %s -> %s", i, \
						lua_typename(L,lua_type(L,-1)), lua_tostring(L,-1) );\
					lua_pop(L,1);\
				} \
			}\
		}

		#include<sstream>
		#include<stdexcept>
		struct lua_State;
		extern "C" int lua_gettop(struct lua_State*);
		struct __SLB__cleanstack
		{
			__SLB__cleanstack(struct lua_State *L, int delta, const char *where, int line)
				: L(L), delta(delta), where(where), line(line)
			{
				top = lua_gettop(L);
			}
			~__SLB__cleanstack()
			{
				if (top+delta != lua_gettop(L))
				{
					std::ostringstream out;
					out << where << ":" << line << " -> ";
					out << "Invalid Stack Check. current = " << lua_gettop(L) << " expected = " << top + delta << std::endl;
					throw std::runtime_error(out.str());
				}
			}
			struct lua_State *L;
			int top;
			int delta;
			const char *where;
			int line;
		};

	#define SLB_DEBUG_CLEAN_STACK(Lua_Stack_L, delta)  \
		__SLB__cleanstack __dummy__SLB_cleanstack_object__(Lua_Stack_L, delta, __FILE__, __LINE__);

#else
	#define SLB_DEBUG(level,...)
	#define SLB_DEBUG_STACK(... ) 
	#define SLB_DEBUG_CLEAN_STACK(...)
#endif

#endif
