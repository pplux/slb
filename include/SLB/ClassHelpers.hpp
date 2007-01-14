#ifndef __SLB_CLASS_HELPERS__
#define __SLB_CLASS_HELPERS__

#include "SPP.hpp"
#include "FuncCall.hpp"

struct lua_State;

namespace SLB {

	template<class C>
	struct Operator
	{
		static C* defaultAdd (const C *a,  const C *b)  { return new C( (*a)+(*b) ); }	
		static C* defaultSub (const C *a,  const C *b)  { return new C( (*a)-(*b) ); }	
		static C* defaultMult(const C *a, const C *b)   { return new C( (*a)*(*b) ); }	
		static C* defaultDiv (const C *a,  const C *b)  { return new C( (*a)/(*b) ); }	
	};

}


#endif
