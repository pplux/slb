#include "unit_002.hpp"
#include<SLB/SLB.hpp>

namespace Unit_002 
{
	bool HybridLock::lockCalled = false;
	bool HybridLock::unlockCalled = false;
	int HybridLock::total = 0;

	HClass::HClass()
	{
		SLB_DEBUG_CALL;
	}


	bool HClass::checkSharedState()
	{
		SLB_DEBUG_CALL;
		return (HybridLock::lockCalled && HybridLock::unlockCalled && HybridLock::total == 0);
	}

	void wrapper()
	{
		SLB_DEBUG_CALL;
		SLB_DEBUG(1, "--> Loading unit_002 wrapper -->");
		
		SLB::Class<HClass>("Unit_002::HClass")
			.constructor()
			.set( "get", &HClass::get )
			.set( "calc", &HClass::calc)
			.set( "checkSharedState", &HClass::checkSharedState)
			.hybrid() //< register basic functionality
		;

		SLB_DEBUG(1, "<-- Loading unit_002 wrapper <--");
	}


	SLB_ON_LOAD(wrapper)

}
