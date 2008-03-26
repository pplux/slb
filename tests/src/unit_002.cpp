#include "unit_002.hpp"
#include<SLB/SLB.hpp>

namespace Unit_002 
{
	int  HClass::get()  { return LCall<int>("get"); }
	void HClass::calc(int a, int b) { return LCall<void, int, int>("calc", a, b); }
	
	void wrapper()
	{
		SLB_DEBUG(1, "--> Loading unit_002 wrapper -->");
		
		SLB::Class<HClass>("Unit_002::HClass")
			.constructor()
			.set( "get", &HClass::get )
			.set( "calc", &HClass::calc)
			.hybrid() //< register basic functionality
		;

		SLB_DEBUG(1, "<-- Loading unit_002 wrapper <--");
	}


	SLB_ON_LOAD(wrapper)

}
