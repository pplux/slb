#ifndef __SLB_HYBRID__
#define __SLB_HYBRID__

#include "SPP.hpp"
#include "Manager.hpp"
#include "LuaCall.hpp"
#include "Value.hpp"
#include "Instance.hpp"
#include <typeinfo>
#include <map>
#include <vector>
#include <string>

struct lua_State;

namespace SLB {

	class Hybrid {
	public:
		Hybrid(lua_State *L = 0L);
		~Hybrid();

		void loadFile(const char *file);
		lua_State *getLuaState() { return _L; }

	protected:
		
	#define SLB_ARG_H(N) ,T##N arg_##N
	#define SLB_ARG(N) , arg_##N
	#define SLB_REPEAT(N) \
	\
		/* LuaCall: functions that return something  */ \
		template<class R SPP_COMMA_IF(N) SPP_ENUM_D(N, class T)> \
		R LCall( const char *name SPP_REPEAT(N, SLB_ARG_H) ) \
		{ \
			SLB::LuaCall<R(SPP_ENUM_D(N,T))> obj; \
			return obj( _L, name SPP_REPEAT(N, SLB_ARG) ); \
		} \

	SPP_MAIN_REPEAT_Z(MAX,SLB_REPEAT)
	#undef SLB_REPEAT
	#undef SLB_ARG
	#undef SLB_ARG_H
	private:
		lua_State *_L;
	};
}

#endif
