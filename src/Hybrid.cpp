/*
    SLB - Simple Lua Binder
    Copyright (C) 2007 Jose L. Hidalgo Valiño (PpluX)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

	Jose L. Hidalgo (www.pplux.com)
	pplux@pplux.com
*/

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

	bool HybridBase::linkFromFile(const char *file)
	{
		bool result = true;
		int t = lua_gettop(_L);
		if(luaL_dofile(_L, file))
		{
			std::cerr << "Error linking from file: " << lua_tostring(_L,-1)
				<< std::endl;
			result = false;
		}
		lua_settop(_L, t);
		return result;
	}

	bool HybridBase::linkFromMemory(const char *buffer)
	{
		bool result = true;
		int t = lua_gettop(_L);
		if(luaL_dostring(_L, buffer))
		{
			std::cerr << "Error linking from Memory: " << lua_tostring(_L,-1)
				<< std::endl;
			/*
			std::cerr
				<< "* Buffer contents: **************************"
				<< std::endl << buffer << std::endl
				<< "*********************************************"
				<< std::endl;
			*/
			result = false;
		}
		lua_settop(_L, t);
		return result;
	}
	
}
