#ifndef __UNIT_002__
#define __UNIT_002__

#include <SLB/Hybrid.hpp>

namespace Unit_002 {


	class HClass : public SLB::Hybrid<HClass>
	{
	public:
		HClass();
		int  get();
		void calc(int a, int b);

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
