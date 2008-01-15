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

#ifndef __SLB_LUACALL__
#define __SLB_LUACALL__

#include "SPP.hpp"
#include "Object.hpp"
#include "PushGet.hpp"
#include "Type.hpp"
#include "lua.hpp"

#include <vector>
#include <typeinfo>
#include <iostream>
#include <stdexcept>

namespace SLB
{

	namespace Private
	{
		class LuaCallBase 
		{ 
		protected: 
			LuaCallBase(lua_State *L, int index);
			LuaCallBase(lua_State *L, const char *func);
			~LuaCallBase();
			void getFunc(int index);
			void execute(int numArgs, int numOutput, int top);

			lua_State *_L;
			int _ref; 
		}; 
	}

	template<typename T>
	struct LuaCall;

	#define SLB_ARG(N) T##N arg_##N, 
	#define SLB_PUSH_ARGS(N) push<T##N>(_L, arg_##N );

	#define SLB_REPEAT(N) \
	\
		/* LuaCall: functions that return something  */ \
		template<class R SPP_COMMA_IF(N) SPP_ENUM_D(N, class T)> \
		struct LuaCall<R( SPP_ENUM_D(N,T) )> : private Private::LuaCallBase\
		{ \
			LuaCall(lua_State *L, int index) : LuaCallBase(L,index) {} \
			LuaCall(lua_State *L, const char *func) : LuaCallBase(L,func) {} \
			R operator()( SPP_REPEAT( N, SLB_ARG) char dummyARG = 0) /*TODO: REMOVE dummyARG */\
			{ \
				R result; \
				int top = lua_gettop(_L); \
				lua_rawgeti(_L, LUA_REGISTRYINDEX,_ref); \
				SPP_REPEAT( N, SLB_PUSH_ARGS ); \
				execute(N, 1, top); \
				result = get<R>(_L, -1); \
				lua_settop(_L,top); \
				return result; \
			} \
			bool operator==(const LuaCall& lc) { return (_L == lc._L && _ref == lc._ref); }\
		};
	SPP_MAIN_REPEAT_Z(MAX,SLB_REPEAT)
	#undef SLB_REPEAT

	#define SLB_REPEAT(N) \
	\
		/*LuaCall: functions that doesn't return anything */  \
		template<SPP_ENUM_D(N, class T)> \
		struct LuaCall<void( SPP_ENUM_D(N,T) )> : private Private::LuaCallBase\
		{ \
			LuaCall(lua_State *L, int index) : LuaCallBase(L,index) {} \
			LuaCall(lua_State *L, const char *func) : LuaCallBase(L,func) {} \
			void operator()( SPP_REPEAT( N, SLB_ARG) char dummyARG = 0) /*TODO: REMOVE dummyARG */\
			{ \
				int top = lua_gettop(_L); \
				lua_rawgeti(_L, LUA_REGISTRYINDEX,_ref); \
				SPP_REPEAT( N, SLB_PUSH_ARGS ); \
				execute(N, 0, top); \
				lua_settop(_L,top); \
			} \
			bool operator==(const LuaCall& lc) { return (_L == lc._L && _ref == lc._ref); }\
		}; \

	SPP_MAIN_REPEAT_Z(MAX,SLB_REPEAT)
	#undef SLB_REPEAT
	#undef SLB_ARG
	#undef SLB_PUSH_ARGS

} //end of SLB namespace

	//--------------------------------------------------------------------
	// Inline implementations:
	//--------------------------------------------------------------------
	
#endif
