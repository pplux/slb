#ifndef __SLB_LUACALL__
#define __SLB_LUACALL__

#include "Object.hpp"
#include "PushGet.hpp"
#include "SPP.hpp"
#include "lua.hpp"

#include <vector>
#include <typeinfo>
#include <iostream>
#include <stdexcept>

namespace SLB
{

	template<typename T>
	struct LuaCall;

	#define SLB_ARG(N) ,T##N arg_##N
	#define SLB_PUSH_ARGS(N) push<T##N>(L, arg_##N );

	#define SLB_REPEAT(N) \
	\
		/* LuaCall: functions that return something  */ \
		template<class R SPP_COMMA_IF(N) SPP_ENUM_D(N, class T)> \
		struct LuaCall<R( SPP_ENUM_D(N,T) )> \
		{ \
			R operator()( lua_State *L , const char *funcname SPP_REPEAT( N, SLB_ARG) ) \
			{ \
				R result; \
				int top = lua_gettop(L); \
				lua_getglobal(L, funcname); \
				SPP_REPEAT( N, SLB_PUSH_ARGS ); \
				if(lua_pcall(L, N, 1, 0)) \
				{ \
					std::runtime_error exception(   \
						std::string(funcname) + " ---lua---> " + lua_tostring(L,-1)); \
					lua_settop(L,top); \
					throw exception;\
				} \
				result = get<R>(L, -1); \
				lua_settop(L,top); \
				return result; \
			} \
		}; \
		/* LuaCall: functions that doesn't return something  */ \
		template<SPP_ENUM_D(N, class T)> \
		struct LuaCall<void( SPP_ENUM_D(N,T) )> \
		{ \
			void operator()( lua_State *L , const char *funcname SPP_REPEAT( N, SLB_ARG) ) \
			{ \
				int top = lua_gettop(L); \
				lua_getglobal(L, funcname); \
				SPP_REPEAT( N, SLB_PUSH_ARGS ); \
				if(lua_pcall(L, N, 0, 0)) \
				{ \
					std::runtime_error exception(   \
						std::string(funcname) + " ---lua---> " + lua_tostring(L,-1)); \
					lua_settop(L,top); \
					throw exception;\
				} \
				lua_settop(L,top); \
			} \
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
