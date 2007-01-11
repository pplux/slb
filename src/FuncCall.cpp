#include <SLB/FuncCall.hpp>
#include <SLB/Debug.hpp>

namespace SLB {

	FuncCall::FuncCall(int items_to_remove) : _Treturn(0), _remove(items_to_remove)
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
		for( int i = 0; i < fc->_remove; ++i) lua_remove(L,1);
		return fc->call(L);
	}

}
