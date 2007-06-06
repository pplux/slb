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

	class HybridBase {
	public:

		bool loadFile(const char *file);
		lua_State *getLuaState() { return _L; }

	protected:
		HybridBase(lua_State *L);
		virtual ~HybridBase();

		lua_State *_L;
	};

	template<class BaseClass>
	class Hybrid : public HybridBase {
	public:
		Hybrid(lua_State *L = 0L) : HybridBase(L) {}
	protected:
		
	#define SLB_ARG_H(N) ,T##N arg_##N
	#define SLB_ARG(N) , arg_##N
	#define SLB_REPEAT(N) \
	\
		/* LuaCall: functions that return something  */ \
		template<class R SPP_COMMA_IF(N) SPP_ENUM_D(N, class T)> \
		R LCall( const char *name SPP_REPEAT(N, SLB_ARG_H) ) \
		{ \
			SLB::LuaCall<R(BaseClass* SPP_COMMA_IF(N) SPP_ENUM_D(N,T))> obj; \
			return obj( _L, name, static_cast<BaseClass*>(this) SPP_REPEAT(N, SLB_ARG) ); \
		} \

	SPP_MAIN_REPEAT_Z(MAX,SLB_REPEAT)
	#undef SLB_REPEAT
	#undef SLB_ARG
	#undef SLB_ARG_H
	};
}

#endif
