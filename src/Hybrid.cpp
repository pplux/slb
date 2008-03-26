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

	HybridBase::HybridBase() : _L(0), _table_ref(-1), _ownState(false)
	{
	}

	HybridBase::~HybridBase()
	{
		clearData();
		if (_L && _ownState)
		{
			lua_close(_L);
		}
	}
	
	void HybridBase::initState()
	{
		clearData();
		_ownState = true;
		_L = luaL_newstate();
		// call user:
		onInitState(_L);
		// add our SLB :)
		Manager::getInstance().registerSLB(_L);
	}
	
	void HybridBase::clearData()
	{
		for(MethodMap::iterator i = _methods.begin(); i != _methods.end(); i++ )
		{
			delete i->second;
		}
		_methods.clear();

		if (_L && _table_ref >= 0) luaL_unref(_L, LUA_REGISTRYINDEX, _table_ref); 
		if (_L  && _ownState ) lua_close(_L);
		_L = 0;
		_table_ref = -1;
	}

	bool HybridBase::link(const char *errMSG)
	{
		/* [top] * there is a function to execute */
		lua_newtable(_L); // independent global table...
		/*ref*/lua_pushvalue(_L, -1); // copy the _G to _table_ref
		/*ref*/_table_ref = luaL_ref(_L, LUA_REGISTRYINDEX); // keep a copy
		lua_setfenv(_L, -2);
		if(lua_pcall(_L, 0, LUA_MULTRET, 0))
		{
			std::cerr << errMSG <<" " << lua_tostring(_L,-1)
				<< std::endl;
			return false;
		}
		return true;
	}

	bool HybridBase::linkFromFile(const char *file)
	{
		initState();
		int t = lua_gettop(_L);
		if (luaL_loadfile(_L,file))
		{
			std::cerr << "Error can not load file " << file << std::endl;
			return false;
		}
		bool result = link("Error linking from file");
		lua_settop(_L, t);
		return result;
	}

	bool HybridBase::linkFromMemory(const char *buffer)
	{
		initState();
		int t = lua_gettop(_L);
		if(luaL_loadstring(_L, buffer))
		{
			std::cerr << "Error can not load from memory [[[" 
				<<std::endl<< buffer << std::endl
				<<"]]]" << std::endl;
			return false;
		}
		bool result = link("Error linking from memory");
		lua_settop(_L, t);
		return result;
	}
	
	bool HybridBase::linkFromLuaTable(lua_State *L, int pos)
	{
		if (L)
		{
			// check there is a valid table:
			if(lua_istable(L, pos))
			{
				clearData();
				// we won't handle the state anymore:
				if (_L && _ownState) lua_close(_L);
				_L = L; _ownState = false;
				lua_pushvalue(L,pos);
				_table_ref = luaL_ref(_L, LUA_REGISTRYINDEX); // keep a copy
				return true;	
			}
		}
		return false;
	}
	
	bool HybridBase::pushFunction(const char *name)
	{
		if (_table_ref > -1)
		{
			lua_rawgeti(_L, LUA_REGISTRYINDEX, _table_ref);
			lua_getfield(_L, -1, name);
			lua_replace(_L, -2);
			if (!lua_isfunction(_L,-1))
			{
				SLB_DEBUG(0, "WARNING: entry [%s] as hybrid method is NOT a function",name);
				lua_pop(_L,1);
				return false;
			}
			// everything correct !
			return true;
		}
		return false;
	}
	
	int HybridBase::lua_linkFromLuaTable(lua_State *L)
	{
		HybridBase *obj = get<HybridBase*>(L,1);
		if (obj)
		{
			if(!obj->linkFromLuaTable(L,2))
			{
				luaL_error(L, "Can not link from table...");
			}
		}
		else
		{
			luaL_error(L, "Invalid Hybrid object");
		}
		return 0;
	}

}
