#include "unit_005.hpp"

namespace Unit_005 {

	SFH::SFH()
	{
		SLB_DEBUG_CALL;
	}

	SFH::~SFH()
	{
		SLB_DEBUG_CALL;
	}

	void wrapper()
	{
		SLB_DEBUG_CALL;
		SLB_DEBUG(1, "--> Loading unit_005 wrapper -->");
		
		SLB::Class< SFH, SLB::Instance::NoCopy >("Unit_005::SFH")
			.inherits<SLB::Script>()
			.constructor()
			.set("doString", &SFH::doString)
			.set("calc", &SFH::calc)
			.hybrid()
		;

		SLB_DEBUG(1, "<-- Loading unit_005 wrapper <--");
	}


	SLB_ON_LOAD(wrapper)

}
