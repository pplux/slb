#include "unit_002.hpp"
#include<SLB/SLB.hpp>

namespace Unit_002 
{
	HClass::HClass() :
		_newStateCalled(false),
		_lockBeginCalled(false),
		_lockEndCalled(false)
	{
	}

	int  HClass::get()  { return LCall<int>("get"); }
	void HClass::calc(int a, int b) { return LCall<void, int, int>("calc", a, b); }

	bool HClass::checkOwnState()
	{
		return (_newStateCalled && !(_lockEndCalled && _lockEndCalled));
	}

	bool HClass::checkSharedState()
	{
		return (!_newStateCalled && (_lockEndCalled && _lockEndCalled));
	}
	
	void HClass::onNewState(lua_State *L)
	{
		luaL_openlibs(L);
		_newStateCalled = true;
	}

	void HClass::lockBegin(lua_State *L)
	{
		_lockBeginCalled = true;
	}

	void HClass::lockEnd(lua_State *L)
	{
		_lockEndCalled = true;
	}
	
	void wrapper()
	{
		SLB_DEBUG(1, "--> Loading unit_002 wrapper -->");
		
		SLB::Class<HClass>("Unit_002::HClass")
			.constructor()
			.set( "get", &HClass::get )
			.set( "calc", &HClass::calc)
			.set( "checkSharedState", &HClass::checkSharedState)
			.set( "checkOwnState", &HClass::checkOwnState)
			.hybrid() //< register basic functionality
		;

		SLB_DEBUG(1, "<-- Loading unit_002 wrapper <--");
	}


	SLB_ON_LOAD(wrapper)

}
