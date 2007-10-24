#ifndef __UNIT_002__
#define __UNIT_002__

#include <SLB/Hybrid.hpp>

namespace Unit_002 {


	class HClass : public SLB::Hybrid<HClass>
	{
	public:
		int  get();
		void calc(int a, int b);
	};

} // end of unit 002

#endif
