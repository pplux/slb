#include "unit_002.hpp"
#include<SLB/SLB.hpp>

namespace Unit_002 
{
	HClass::HClass() :
		_lockBeginCalled(false),
		_lockEndCalled(false),
		_total(0)
	{
	}


	bool HClass::checkSharedState()
	{
		return (_lockBeginCalled && _lockEndCalled && _total == 0);
	}

	void HClass::lockBegin(lua_State *L)
	{
		std::cout << "Called LOCK BEGIN" << std::endl;
		_lockBeginCalled = true;
		_total++;
	}

	void HClass::lockEnd(lua_State *L)
	{
		std::cout << "Called LOCK END" << std::endl;
		_lockEndCalled = true;
		_total--;
	}
	
	void wrapper()
	{
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
