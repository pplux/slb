#ifndef __UNIT_002__
#define __UNIT_002__

#include <SLB/Hybrid.hpp>

namespace Unit_002 {

	struct HybridLock : public SLB::ActiveWaitCriticalSection
	{
		HybridLock(SLB::Mutex *m) : SLB::ActiveWaitCriticalSection(m) { lockCalled = true; total++;  }
		~HybridLock() { unlockCalled = true; total--;}

		static bool lockCalled;
		static bool unlockCalled;
		static int total;
	};

	class HClass : public SLB::Hybrid<HClass, HybridLock>
	{
	public:
		HClass();

		/* int get() { return LCall<int>("get"); } */
		HYBRID_method_0(get,int);

		/* void calc(int a, int b) { return LCall<void, int, int>("calc"); } */
		HYBRID_method_2(calc,void,int,int);

		HYBRID_const_method_0(update,void);

		bool checkSharedState(); //< should be true when linking from a lua_table

	private:
		int _total;
	};

} // end of unit 002

#endif
