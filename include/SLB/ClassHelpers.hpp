#ifndef __SLB_CLASS_HELPERS__
#define __SLB_CLASS_HELPERS__

#include "SPP.hpp"
#include "FuncCall.hpp"

struct lua_State;

namespace SLB {

	template<class C>
	struct Operator
	{
		static C* defaultAdd(const C *a,  const C *b)  { return new C( (*a)+(*b) ); }	
		static C* defaultSub(const C *a,  const C *b)  { return new C( (*a)+(*b) ); }	
		static C* defaultMult(const C *a, const C *b)  { return new C( (*a)+(*b) ); }	
		static C* defaultDiv(const C *a,  const C *b)  { return new C( (*a)+(*b) ); }	
	};

	template<class T>
	inline void Destructor(void *raw, lua_State *)
	{
		T *obj = reinterpret_cast<T*>(raw);
		delete obj;
	};

	template<class T>
	struct Constructor;

	template<class C>	
	struct Constructor<C()>
	{
		static FuncCall* create() { FuncCall* fc = FuncCall::create(createInstance); fc->setRemoveItems(1); return fc; }
		static C* createInstance()
		{
			return new C();
		}
	};
	
	#define SLB_CONSTRUCTOR_PARAM(N) , T##N param_##N
	#define SLB_REPEAT(N) \
		template<class C, class T0 SPP_COMMA_IF(N) SPP_ENUM_D(N, class T)> \
		struct Constructor<C(T0 SPP_COMMA_IF(N) SPP_ENUM_D(N,T))> \
		{\
			static FuncCall* create() { FuncCall* fc = FuncCall::create(createInstance); fc->setRemoveItems(1); return fc; } \
			static C* createInstance( T0 param_0 SPP_REPEAT(N,SLB_CONSTRUCTOR_PARAM) ) \
			{ \
				return new C( param_0 SPP_COMMA_IF(N) SPP_ENUM_D(N, param_) ); \
			} \
		}; \

	SPP_MAIN_REPEAT_Z(MAX,SLB_REPEAT)
	#undef SLB_REPEAT
	#undef SLB_CONSTRUCTOR_PARAM

}


#endif
