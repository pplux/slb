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

#include <SLB/Manager.hpp>
#include <SLB/ClassInfo.hpp>
#include <SLB/lua.hpp>
#include <SLB/Debug.hpp>
#include <SLB/util.hpp>

#include <iostream>

namespace SLB {

	/* Global functions */
	int SLB_type(lua_State *L)
	{
		const ClassInfo *ci = Manager::getInstance().getClass(L,-1);
		if (ci)
		{
			lua_pushstring(L, ci->getName().c_str());
			return 1;
		}
		return 0;
	}

	int SLB_rawptr(lua_State *L)
	{
		int top = lua_gettop(L);
		if (lua_getmetatable(L,1))
		{
			lua_getfield(L, -1, "__class_ptr");
			if (!lua_isnil(L,-1))
			{
				ClassInfo* ci = reinterpret_cast<ClassInfo*>( lua_touserdata(L,-1) );
				const void* raw = ci->get_const_ptr(L, 1);
				lua_settop(L, top);
				lua_pushinteger(L, (lua_Integer) raw);
				return 1;
			}
		}
		lua_settop(L, top);
		return 0;
	}

	int SLB_copy(lua_State *L)
	{
		int top = lua_gettop(L);
		if (lua_getmetatable(L,1))
		{
			lua_getfield(L, -1, "__class_ptr");
			if (!lua_isnil(L,-1))
			{
				ClassInfo* ci = reinterpret_cast<ClassInfo*>( lua_touserdata(L,-1) );
				lua_settop(L, top);
				ci->push_copy(L, lua_touserdata(L,1));
				return 1;
			}
		}
		lua_settop(L, top);
		return 0;
	}

	int SLB_using_index(lua_State *L)
	{
		lua_pushnil(L);
		while( lua_next(L, lua_upvalueindex(1)) )
		{
			lua_pushvalue(L,2); // key
			lua_gettable(L, -2); 
			if (!lua_isnil(L,-1))
			{
				return 1;
			}
			lua_pop(L,2);
		}
		return 0;
	}

	int SLB_using(lua_State *L)
	{
		int top = lua_gettop(L);
		luaL_checktype(L, 1, LUA_TTABLE);

		lua_getfield(L, LUA_REGISTRYINDEX, "SLB_using");
		if ( lua_isnil(L,-1) )
		{
			// pop nil value
			lua_pop(L,1);

			// get or create _G's metatable 
			if(!lua_getmetatable(L, LUA_GLOBALSINDEX))
			{
				lua_newtable(L);
				// set as metatable of _B 
				lua_pushvalue(L,-1);
				lua_setmetatable(L, LUA_GLOBALSINDEX);
			}
			else
			{
				luaL_error(L, "Can not use SLB.using,"
					" _G already has a metatable");
			}

			lua_newtable(L); // create the "SLB_using" table
			lua_pushvalue(L,-1); // keep a copy in registry 
			lua_setfield(L, LUA_REGISTRYINDEX, "SLB_using");

			// push the using_index func with the SLB_using table as closure
			lua_pushvalue(L,-1);
			lua_pushcclosure(L, SLB_using_index, 1); // push the closure
			// set this functions as "__index" of the metatable (at -3)
			lua_setfield(L, -3, "__index");

			// leave the SLB_using table at the top
		}
		lua_pushvalue(L,1); // get the first argument
		//TODO check not repeating tables...
		luaL_ref(L, -2); // add the table

		lua_settop(L,top);
		return 0;
	}

	int SLB_isA(lua_State *L)
	{
		int top = lua_gettop(L);
		if (top != 2)
			luaL_error(L, "Invalid number of arguments (instance, class)");

		ClassInfo* ci_a = 0;
		if (lua_getmetatable(L,1))
		{
			lua_getfield(L, -1, "__class_ptr");
			if (!lua_isnil(L,-1))
			{
				ci_a = reinterpret_cast<ClassInfo*>( lua_touserdata(L,-1) );
			}
		}
		ClassInfo* ci_b = 0;
		if (lua_getmetatable(L,2))
		{
			lua_getfield(L, -1, "__class_ptr");
			if (!lua_isnil(L,-1))
			{
				ci_b = reinterpret_cast<ClassInfo*>( lua_touserdata(L,-1) );
			}
		}
		lua_settop(L, top);
		if ( ci_a && ci_b ) 
		{
			lua_pushboolean(L, ci_a->isSubClassOf(ci_b) );
		}
		else
		{
			lua_pushboolean(L, false);
		}
		return 1;
	}

	static const luaL_Reg SLB_funcs[] = {
		{"type", SLB_type},
		{"copy", SLB_copy},
		{"using", SLB_using},
		{"rawptr", SLB_rawptr},
		{"isA", SLB_isA},
		{NULL, NULL}
	};

	Manager* Manager::_singleton = 0;

	Manager::Manager()
	{
		SLB_DEBUG(0, "Manager initialization");
		_global = new Namespace();
	}

	Manager::~Manager()
	{
		SLB_DEBUG(0, "Manager destruction");
	}
	
	void Manager::registerSLB(lua_State *L)
	{
		int top = lua_gettop(L);

		// Register global functions
		luaL_register(L, "SLB", SLB_funcs);
		// metatable of "SLB"
		lua_newtable(L);
		lua_pushstring(L,"__index");
		_global->push(L);
		lua_rawset(L,-3);
		lua_setmetatable(L,-2); // SLB table

		lua_settop(L,top);
	}
	
	Manager *Manager::getInstancePtr()
	{
		static bool _atexit = false;
		if (_atexit == false)
		{
			_atexit = true;
			atexit(Manager::reset);
		}
		if (_singleton == 0) _singleton = new Manager();
		return _singleton;
	}

	void Manager::reset()
	{
		delete _singleton;
		_singleton = 0;
	}
	
	void Manager::addClass( ClassInfo *c )
	{
		_classes[ c->getTypeid() ] = c;
	}

	const ClassInfo *Manager::getClass(const std::type_info &ti) const
	{
		ClassMap::const_iterator i = _classes.find(&ti);
		if ( i != _classes.end() ) return i->second.get();
		return 0;
	}

	const ClassInfo *Manager::getClass(const std::string &name) const
	{
		NameMap::const_iterator i = _names.find(name);
		if ( i != _names.end() ) return getClass( *i->second );
		return 0;
	}

	ClassInfo *Manager::getClass(lua_State *L, int pos) const
	{
		pos = L_abs_index(L,pos);
		int top = lua_gettop(L);
		ClassInfo* ci = 0L;
		if (lua_getmetatable(L,pos))
		{
			lua_getfield(L, -1, "__class_ptr");
			if (!lua_isnil(L,-1))
			{
				ci = reinterpret_cast<ClassInfo*>( lua_touserdata(L,-1) );
			}
		}
		lua_settop(L, top);
		return ci;
	}


	ClassInfo *Manager::getClass(const std::string &name)
	{
		NameMap::iterator i = _names.find(name);
		if ( i != _names.end() ) return getClass( *i->second );
		return 0;
	}

	ClassInfo *Manager::getClass(const std::type_info &ti)
	{
		ClassMap::iterator i = _classes.find(&ti);
		if ( i != _classes.end() ) return i->second.get();
		return 0;
	}
		

	ClassInfo *Manager::getOrCreateClass(const std::type_info &ti)
	{
		ClassInfo *c = 0;
		ClassMap::iterator i = _classes.find(&ti);
		if ( i != _classes.end() )
		{
			c = i->second.get();
		}
		else
		{
			c = new ClassInfo(ti);
		}
		return c;
	}
	
	void Manager::set(const std::string &name, Object *obj)
	{
		_global->set(name, obj);
	}
	
	void Manager::rename(ClassInfo *ci, const std::string &new_name)
	{
		const std::string old_name = ci->getName();

		NameMap::iterator i = _names.find(old_name);
		if ( i != _names.end() )
		{
			_global->erase(old_name);
			_names.erase(i);
		}

		_global->set(new_name, ci);
		_names[ new_name ] = ci->getTypeid();

	}
}

