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
	#include <cstring>
	#define SLB_DEBUG_FUNC(...) fprintf(SLB_DEBUG_OUTPUT, __VA_ARGS__);
#endif

//----------------------------------------------------------------------------
//-- Simple Debug (dissabled by default) -------------------------------------
//----------------------------------------------------------------------------
#ifndef SLB_DEBUG_LEVEL
#define SLB_DEBUG_LEVEL 0
#endif
	
#if SLB_DEBUG_LEVEL != 0
	#include<SLB/SPP.hpp>
	#include<SLB/Export.hpp>

	extern SLB_EXPORT int SLB_DEBUG_LEVEL_TAB;

	inline void __SLB_ADJUST__(bool terminator = true)
	{
		if (SLB_DEBUG_LEVEL_TAB)
		{
			for(int i = 0; i < SLB_DEBUG_LEVEL_TAB-1; ++i) SLB_DEBUG_FUNC("| ");
			if (terminator) SLB_DEBUG_FUNC("|_");
		}
	}

	#define SLB_DEBUG(level,...) if (level <= SLB_DEBUG_LEVEL)\
		{\
			__dummy__SLB__debugcall.check_SLB_DEBUG_CALL(); /* to check a previous SLB_DEBUG_CALL */ \
			int __s = strlen(__FILE__); \
			int __offset = (__s > 18)? __s - 18: 0; \
			__SLB_ADJUST__();\
			SLB_DEBUG_FUNC("SLB-%-2d [%12s:%-4d] ", level, __FILE__+__offset, __LINE__);\
			SLB_DEBUG_FUNC(__VA_ARGS__);\
			SLB_DEBUG_FUNC("\n");\
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
					SLB_DEBUG(level+1, "\targ %d = %s -> %s", i, \
						lua_typename(L,lua_type(L,-1)), lua_tostring(L,-1) );\
					lua_pop(L,1);\
				} \
			}\
		}

		#include<sstream>
		#include<stdexcept>
		#include "lua.hpp"
	
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
		__SLB__cleanstack __dummy__SLB__cleanstack__(Lua_Stack_L, delta, __FILE__, __LINE__);

		struct __SLB__debugcall
		{
			__SLB__debugcall(const char *f, int l, const char *n)
				: file(f), line(l), name(n)
			{
				int s = ::strlen(__FILE__);
				int offset = (s > 18)? s - 18: 0;
				file = file + offset;
				__SLB_ADJUST__();
				SLB_DEBUG_FUNC("SLB >>> [%12s:%-4d] %s\n", file, line, name);
				SLB_DEBUG_LEVEL_TAB++;
			}

			~__SLB__debugcall()
			{
				__SLB_ADJUST__();
				SLB_DEBUG_FUNC("SLB <<< [%12s:%-4d] %s\n", file, line, name);
				__SLB_ADJUST__(false);
				SLB_DEBUG_FUNC("\n");
				SLB_DEBUG_LEVEL_TAB--;
			}

			void check_SLB_DEBUG_CALL() const {}


			const char *file;
			int line;
			const char *name;

		};
	#define SLB_DEBUG_CALL \
		__SLB__debugcall __dummy__SLB__debugcall(__FILE__,__LINE__,__FUNCTION__);

#else
	#define SLB_DEBUG(level,...)
	#define SLB_DEBUG_STACK(... ) 
	#define SLB_DEBUG_CLEAN_STACK(...)
	#define SLB_DEBUG_CALL
#endif

#endif
