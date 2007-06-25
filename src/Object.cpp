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

#include <SLB/Object.hpp>
#include <SLB/Debug.hpp>
#include <SLB/lua.hpp>
#include <SLB/Manager.hpp>

#include<assert.h>

namespace SLB {
		
	const char *objectsTable_name = "SLB_Objects";
	const char *refTable_name     = "SLB_References";

	Object::Object() : _refCounter(0)
	{
	}

	Object::~Object()
	{
	}
	
	void Object::initialize(lua_State *L)
	{
		int top = lua_gettop(L);

		// create a new objects' table
		lua_newtable(L); 
		//make it weak(-value)
		lua_newtable(L);
		lua_pushstring(L, "v");
		lua_setfield(L, -2, "__mode");
		lua_setmetatable(L,-2);
		lua_setfield(L, LUA_REGISTRYINDEX, objectsTable_name); 

		// create new references' table
		lua_newtable(L); // create a new table
		//make it weak(-key)
		lua_newtable(L);
		lua_pushstring(L, "k");
		lua_setfield(L, -2, "__mode");
		lua_setmetatable(L,-2);
		lua_setfield(L, LUA_REGISTRYINDEX, refTable_name); 

		lua_settop(L, top);
	}
	
	void Object::push(lua_State *L)
	{
		SLB_DEBUG(3, "(L %p) Object::push(%p) [%s]", L, this, typeid(*this).name());
		int top = lua_gettop(L);

		// get the table of objects (top+1)
		lua_getfield(L, LUA_REGISTRYINDEX, objectsTable_name);
		if (lua_isnil(L, -1))
		{
			lua_pop(L, 1); // remove nil
			initialize(L);
			lua_getfield(L, LUA_REGISTRYINDEX, objectsTable_name);
		}
		// get Reference's table (top+2)
		lua_getfield(L, LUA_REGISTRYINDEX, refTable_name);

		// find the object with our id
		lua_pushlightuserdata(L, this);
		lua_rawget(L, top + 1);

		if (lua_isnil(L,-1))
		{
			lua_pop(L, 1); // remove nil

			int objpos = lua_gettop(L) + 1; // position of the user pushImplementation
			pushImplementation(L); // value (0)
			SLB_DEBUG(5, "\t-new object");

			// check, pushImplementation should return only one value
			if (lua_gettop(L) != objpos) 
			{
				luaL_error(L, "Error on Object::push the current stack "
					"has %d elments and should have only one.", 
					lua_gettop(L) - objpos - 1);
				lua_settop(L, top);
				return;
			}

			// make objects[key] = value
			lua_pushlightuserdata(L, this); // key
			lua_pushvalue(L, objpos); // copy of value (0)
			lua_rawset(L, top+1); // objects[key] = value	


			lua_pushvalue(L, objpos); // key 
			// create a reference of this object... 
			Object **objptr = reinterpret_cast<Object**>(
				lua_newuserdata(L, sizeof(Object*)));
			*objptr = this; // copy a pointer
			ref(); // manual reference

			// get or crate a metatable for the reference of this
			if(luaL_newmetatable(L, "SLB_ObjectPtr_GC"))
			{
				// create the __gc function
				lua_pushcfunction(L, GC_callback);
				lua_setfield(L, -2, "__gc"); // set the gc func of the obj
			}
			lua_setmetatable(L, -2); // metatable of reference object

			// make references[obj] = reference
			lua_rawset(L, top+2);

			// check object at top
			assert( lua_gettop(L) == objpos);
		}
		else
		{
			SLB_DEBUG(6, "\t-object exists");
		}

		// replace top+1 with the object
		lua_replace(L, top+1);
		lua_settop(L, top+1);
	}
	
	int Object::GC_callback(lua_State *L)
	{
		Object *obj = *reinterpret_cast<Object**>(lua_touserdata(L, 1));
		SLB_DEBUG(2, "(L %p) GC object %p (refcount %d - 1) [%s]", L, obj, obj->referenceCount(), typeid(*obj).name());
		obj->onGarbageCollection(L);
		obj->unref(); // manual unref
		return 0;
	}

}
