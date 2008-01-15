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

#include <SLB/LuaCall.hpp>
#include <SLB/Debug.hpp>
#include <sstream>

namespace SLB {
namespace Private {

	LuaCallBase::LuaCallBase(lua_State *L, int index) : _L(L) { getFunc(index); } 

	LuaCallBase::LuaCallBase(lua_State *L, const char *func) : _L(L) 
	{
		lua_getglobal(L,func);
		getFunc(-1);
		lua_pop(L,1); 
	}

	LuaCallBase::~LuaCallBase()
	{
		luaL_unref(_L, LUA_REGISTRYINDEX, _ref); 
	}

	void LuaCallBase::getFunc(int index)
	{
		lua_pushvalue(_L,index);
		_ref = luaL_ref(_L, LUA_REGISTRYINDEX);
	}

	int LuaCallBase::errorHandler(lua_State *L)
	{
		std::ostringstream out; // Use lua pushfstring and so on...
		lua_Debug debug;

		out << "SLB Exception: "
			<< std::endl << "-------------------------------------------------------"
			<< std::endl;
		out << "Lua Error:" << std::endl << "\t" 
			<<  lua_tostring(L, -1) << std::endl
			<< "Traceback:" << std::endl;
		for ( int level = 0; lua_getstack(L, level, &debug ); level++)
		{
			if (lua_getinfo(L, "Sln", &debug) )
			{
				//TODO use debug.name and debug.namewhat
				//make this more friendly
				out << "\t [ " << level << " (" << debug.what << ") ] ";
				if (debug.currentline > 0 )
				{
					out << debug.short_src << ":" << debug.currentline; 
					if (debug.name)
						out << " @ " << debug.name << "(" << debug.namewhat << ")";
				}
				out << std::endl;
			}
			else
			{
				out << "[ERROR using Lua DEBUG INTERFACE]" << std::endl;
			}
		}

		lua_pushstring(L, out.str().c_str()) ;
		return 1;
	}

	void LuaCallBase::execute(int numArgs, int numOutput, int top)
	{
		int base = lua_gettop(_L) - numArgs;
		lua_pushcfunction(_L, LuaCallBase::errorHandler);
		lua_insert(_L, base);

		if(lua_pcall(_L, numArgs, numOutput, base)) 
		{
			std::runtime_error exception( lua_tostring(_L, -1) );
			lua_remove(_L, base);
			lua_settop(_L,top); // TODO: Remove this.
			throw exception;
		}
		lua_remove(_L, base);
	}

}}
