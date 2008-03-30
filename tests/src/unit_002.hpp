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

		bool checkOwnState(); //< should be true when linking from disk or memory
		bool checkSharedState(); //< should be true when linking from a lua_table

	protected:
		void onNewState(lua_State *L);
		void lockBegin(lua_State *L);
		void lockEnd(lua_State *L);
	private:
		bool _newStateCalled;
		bool _lockBeginCalled;
		bool _lockEndCalled;
	};

} // end of unit 002

#endif
