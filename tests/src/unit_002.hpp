#ifndef __UNIT_002__
#define __UNIT_002__

#include <SLB/Hybrid.hpp>

namespace Unit_002 {


	class HClass : public SLB::Hybrid<HClass>
	{
	public:
		HClass();

		/* int get() { return LCall<int>("get"); } */
		HYBRID_method_0(get,int);

		/* void calc(int a, int b) { return LCall<void, int, int>("calc"); } */
		HYBRID_method_2(calc,void,int,int);

		HYBRID_const_method_0(update,void);

		bool checkSharedState(); //< should be true when linking from a lua_table

	protected:
		void lockBegin(lua_State *L);
		void lockEnd(lua_State *L);

	private:
		bool _lockBeginCalled;
		bool _lockEndCalled;
		int _total;
	};

} // end of unit 002

#endif
