#include <SLB/FuncCall.hpp>
#include <SLB/Debug.hpp>

namespace SLB {

	FuncCall::FuncCall() : _Treturn(0)
	{
		SLB_DEBUG(10, "Create FuncCall (%p)",this);
	}

	FuncCall::~FuncCall()
	{
		SLB_DEBUG(10, "Delete FuncCall (%p)",this);
	}

	
	void FuncCall::pushImplementation(lua_State *L)
	{
		lua_pushlightuserdata(L, (FuncCall*) this);
		lua_pushcclosure(L,FuncCall::_call, 1);
	}
	
	int FuncCall::_call(lua_State *L)
	{
		FuncCall *fc = (FuncCall*) lua_touserdata(L,lua_upvalueindex(1));
		return fc->call(L);
	}

}
