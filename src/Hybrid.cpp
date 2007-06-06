#include <SLB/Hybrid.hpp>
#include <SLB/lua.hpp>
#include <SLB/Manager.hpp>

#include <iostream>

namespace SLB {

	HybridBase::HybridBase(lua_State *L) : _L(L)
	{
		if (_L == 0)
		{
			_L = luaL_newstate();
			luaL_openlibs(_L);
		}
		Manager::getInstance().registerSLB(_L);
		// counter...
		lua_getfield(_L, LUA_REGISTRYINDEX, "SLB_HYBRID");
		if (lua_isnil(_L,-1))
		{
			lua_pop(_L,1);
			lua_pushinteger(_L,0);
		}
		int v = lua_tointeger(_L,-1);
		lua_pushinteger(_L,v+1);
		lua_setfield(_L, LUA_REGISTRYINDEX, "SLB_HYBRID");
		lua_pop(_L,1);
	}

	HybridBase::~HybridBase()
	{
		lua_getfield(_L, LUA_REGISTRYINDEX, "SLB_HYBRID");
		int v = lua_tointeger(_L,-1);
		if ( v > 1 )
		{
			lua_pop(_L,1);
			lua_pushinteger(_L, v-1);
			lua_setfield(_L, LUA_REGISTRYINDEX, "SLB_HYBRID");
		}
		else
		{
			lua_close(_L);
		}
	}

	bool HybridBase::loadFile(const char *file)
	{
		bool result = true;
		int t = lua_gettop(_L);
		if(luaL_dofile(_L, file))
		{
			std::cerr << "Error loading file: " << lua_tostring(_L,-1)
				<< std::endl;
			result = false;
		}
		lua_settop(_L, t);
		return result;
	}
	
}
