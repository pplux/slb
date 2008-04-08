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
#include <sstream>
#include <iostream>

namespace SLB {

	/*--- Invalid Method (exception) ----------------------------------------------*/
 	InvalidMethod::InvalidMethod(lua_State  *L, const char *c)
	{
		std::ostringstream out;
		lua_Debug debug;

		out << "Invalid Method '" << c << "' NOT FOUND!" << std::endl;
		out << "TraceBack:" << std::endl;
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
		out << "Current Stack:" << std::endl;
		for(int i = 1; i < lua_gettop(L); ++i)
		{
			out << "\t ["<<i<<"] " << lua_typename(L, lua_type(L,i))
				<< " : "<< lua_tostring(L,i) ;
			ClassInfo *ci = Manager::getInstance().getClass(L,i);
			if (ci) out << "->" << ci->getName();
			out << std::endl;
		}
		
		_what = out.str();
	}
	/*--- Invalid Method (exception) ----------------------------------------------*/

	/*--- HybridBase::AutoLock ----------------------------------------------------*/
	HybridBase::AutoLock::AutoLock(const HybridBase *hconst) 
	{
		//TODO: Review this! (should be const?)
		_hybrid = const_cast<HybridBase*>(hconst);
		SLB_DEBUG(6, "Lock state %p to access hybrid method (%p)",
			_hybrid->_L, (void*) _hybrid);
		_hybrid->lockBegin( _hybrid->_L );
	}

	HybridBase::AutoLock::~AutoLock()
	{
		SLB_DEBUG(6, "Unlock state %p to access hybrid method (%p)",
			_hybrid->_L, (void*) _hybrid);
		_hybrid->lockEnd( _hybrid->_L);
	}
	/*--- HybridBase::AutoLock ----------------------------------------------------*/

	/*--- Internal Hybrid Subclasses ----------------------------------------------*/
	struct InternalHybridSubclass : public Table
	{
		InternalHybridSubclass(ClassInfo *ci) : _CI(ci)
		{
			assert("Invalid ClassInfo" && _CI.valid());
		}

		int __newindex(lua_State *L)
		{
			SLB_DEBUG_STACK(6,L, "Call InternalHybridSubclass(%p)::__nexindex", this);
			//1 = table
			//2 = string
			//3 = function
			luaL_checkstring(L,2);
			if (lua_type(L,3) != LUA_TFUNCTION)
			{
				luaL_error(L, "Only functions can be added to hybrid classes"
					" (invalid %s of type %s)", lua_tostring(L,2), lua_typename(L, 3));
			}
			SLB_DEBUG(4, "Added method to an hybrid-subclass:%s", lua_tostring(L,2));
			lua_pushcclosure(L, HybridBase::call_lua_method, 1); // replaces 
			setCache(L);
			return 0;
		}

		int __call(lua_State *L)
		{
			SLB_DEBUG_STACK(6,L, "Call InternalHybridSubclass(%p)::__call", this);
			// create new instance:
			ref_ptr<FuncCall> fc = _CI->getConstructor();
			assert("Invalid Constructor!" && fc.valid());
			fc->push(L);
			lua_replace(L,1); // table of metamethod __call
			lua_call(L, lua_gettop(L) -1 , LUA_MULTRET);
			// at 1 we should have an HybridBase instance...
			HybridBase *obj = SLB::get<HybridBase*>(L,1);
			if (obj == 0) luaL_error(L, "Output(1) of constructor should be an HybridBase instance");
			// now our table... to find methods
			obj->_subclassMethods = this;
			return lua_gettop(L);
		}

	private:
		ref_ptr<ClassInfo> _CI;
	};
	/*--- Internal Hybrid Subclasses ----------------------------------------------*/


	HybridBase::HybridBase() : _L(0),
		_table_globals(0)
	{
	}

	HybridBase::~HybridBase()
	{
		clearMethodMap();
		if (_L)
		{
			luaL_unref(_L, LUA_REGISTRYINDEX, _table_globals);
			_L = 0;
			_table_globals = 0;
		}
	}

	void HybridBase::attach(lua_State *L)
	{
		//TODO allow reattaching...
		if (_L) throw std::runtime_error("Trying to reattach an Hybrid instance");

		if (L)
		{
			_L = L;
			lua_newtable(_L);
			// use "global" state as __index to access outside values like
			// normal functions. And always try to reuse it to every instance.
			lua_getfield(_L, LUA_REGISTRYINDEX, "__SLB_HYBRID_GLOBALS__");
			if (lua_isnil(_L,-1))
			{
				lua_pop(_L,1); // [-1] remove nil
				lua_newtable(_L); // [+1] meta-table
				lua_pushvalue(_L, LUA_GLOBALSINDEX); // [+1] globals _G
				lua_setfield(_L, -2, "__index"); // [-1]
				lua_pushvalue(_L,-1); // [+1] a copy for registry
				lua_setfield(_L, LUA_REGISTRYINDEX, "__SLB_HYBRID_GLOBALS__"); // [-1]
			}
			lua_setmetatable(L,-2);
			// done

			_table_globals = luaL_ref(_L, LUA_REGISTRYINDEX);
		}
	}
	
	void HybridBase::clearMethodMap()
	{
		// delete the list of _methods
		for(MethodMap::iterator i = _methods.begin(); i != _methods.end(); i++ )
		{
			delete i->second;
		}
		_methods.clear();
	}
	
	bool HybridBase::getMethod(const char *name) const
	{
		SLB_DEBUG(5, "HybridBase(%p)::getMethod '%s' (_L = %p)", this, name, _L); 
		if (_L == 0) throw std::runtime_error("Hybrid instance not attached");\
		int top = lua_gettop(_L);
		ClassInfo *ci = getClassInfo();
		ci->push(_L);
		lua_getmetatable(_L,-1);
		lua_getfield(_L,-1, "__hybrid");
		if (!lua_isnil(_L,-1))
		{
			lua_pushstring(_L,name);
			lua_rawget(_L,-2);
			if (!lua_isnil(_L,-1))
			{
				lua_replace(_L,top+1);
				lua_settop(_L,top+1);
				SLB_DEBUG(6, "HybridBase(%p-%s)::getMethod '%s' (_L = %p) -> FOUND",
						this, ci->getName().c_str(),name, _L); 
				return true;
			}
			else SLB_DEBUG(6, "HybridBase(%p-%s)::getMethod '%s' (_L = %p) -> *NOT* FOUND",
				this,ci->getName().c_str(), name, _L); 
		}
		else SLB_DEBUG(4, "HybridBase(%p-%s) do not have any hybrid methods", this, ci->getName().c_str());

		// anyway... if not found:
		lua_settop(_L,top);
		return false;
	}
	
	void HybridBase::setMethod(lua_State *L, ClassInfo *ci)
	{
		// key
		// value [top]
		int top = lua_gettop(L);
		// checks key, value
		assert( "Invalid key for method" && lua_type(L,top-1) == LUA_TSTRING);
		assert( "Invalid type of method" && lua_type(L,top) == LUA_TFUNCTION);

		ci->push(L); // top +1
		lua_getmetatable(L,-1); // top +2
		lua_getfield(L,-1, "__hybrid"); // top +3
		// create if not exists
		if (lua_isnil(L,-1))
		{
			lua_pop(L,1); // remove nil
			lua_newtable(L); // top +3
			lua_pushstring(L, "__hybrid");
			lua_pushvalue(L,-2); // a copy for ClassInfo
			lua_rawset(L, top+2); // set to he metatable 
		}
		lua_insert(L,top-2); // put the __hybrid table below key,value
		lua_settop(L, top+1); // table, key, and value
		lua_rawset(L,top-2); // set elements
		lua_settop(L, top-2); // remove everything :)
	}

	void HybridBase::registerAsHybrid(ClassInfo *ci)
	{
		//TODO: Check first if the class already has __index, __newindex
		//Maybe this should be a feature at ClassInfo to warn in case
		//the user sets these functions more than once.
		ci->setClass__newindex( FuncCall::create(class__newindex) );
		ci->setClass__index( FuncCall::create(class__index) );
		ci->setObject__index( FuncCall::create(object__index) );
		ci->setHybrid();
	}
	

	HybridBase* get_hybrid(lua_State *L, int pos)
	{
		HybridBase *obj = get<HybridBase*>(L,pos);
		if (!obj)
		{
			luaL_error(L, "Invalid Hybrid object");
		}
		return obj;
	}


	int HybridBase::call_lua_method(lua_State *L)
	{
		HybridBase *hb = get<HybridBase*>( L, 1 );
		if (hb == 0) luaL_error(L, "Invalid hybrid object");
		if (hb->_L == 0) luaL_error(L, "Instance(%p) not attached to any lua_State...", hb);
		if (hb->_L != L) luaL_error(L, "This instance(%p) is attached to another lua_State(%p)", hb, hb->_L);
		// get the real function to call
		lua_pushvalue(L, lua_upvalueindex(1));
		// get the environment (from object) and set it
		lua_rawgeti(L, LUA_REGISTRYINDEX, hb->_table_globals);
		lua_setfenv(L,-2);
		lua_insert(L,1); //put the target function at 1
		SLB_DEBUG_STACK(10, L, "Hybrid(%p)::call_lua_method ...", hb);
		lua_call(L, lua_gettop(L) - 1, LUA_MULTRET);
		return lua_gettop(L);
	}

	int HybridBase::class__newindex(lua_State *L)
	{
		// 1 - obj (table with classInfo)
		ClassInfo *ci = Manager::getInstance().getClass(L,1);
		if (ci == 0) luaL_error(L, "Invalid Class at #1");

		// 2 - key (string)
		const int key = 2;

		// 3 - value (func)
		const int value = 3;

		if (lua_isstring(L,key) && lua_isfunction(L,value))
		{
			// create a closure with the function to call
			lua_pushcclosure(L, HybridBase::call_lua_method, 1); // replaces [value]
			setMethod(L, ci);	
		}
		else
		{
			luaL_error(L,
				"hybrid instances can only have new methods (functions) "
				"indexed by strings ( called with: class[ (%s) ] = (%s) )",
				lua_typename(L, lua_type(L,key)), lua_typename(L, lua_type(L,value))
				);
		}
		return 0;
	}

	int HybridBase::object__index(lua_State *L)
	{
		SLB_DEBUG(4, "HybridBase::object__index");
		// 1 - obj (table with classInfo)
		HybridBase* obj = get<HybridBase*>(L,1);
		if (obj == 0) luaL_error(L, "Invalid instance at #1");
		if (!obj->_L) luaL_error(L, "Hybrid instance not attached or invalid method");
		if (obj->_L != L) luaL_error(L, "Can not use that object outside its lua_state(%p)", obj->_L);

		// 2 - key (string) (at top)
		const char *key = lua_tostring(L,2);
		if (obj->_subclassMethods.valid())
		{
			//TODO: NEED DEBUG...
			// ---- why not:
			// Object *m = obj->_subclassMethods->get(key);
			// if (m)
			// {
			// 	SLB_DEBUG(5, "Hybrid subclassed instance, looking for '%s' method [OK]", key);
			// 	m->push(L);
			// 	return 1;
			// }
			// else SLB_DEBUG(5, "Hybrid subclassed instance, looking for '%s' method [FAIL!]", key);
			// ---- instead of: (even though this is quicker) but code above should work
			lua_pushvalue(L,2); // [+1]
			obj->_subclassMethods->getCache(L); // [-1, +1] will pop key's copy and return the cache 
			if (!lua_isnil(L,-1)) return 1;
			lua_pop(L,1); // [-1] remove nil
			//TODO:....................................................
		}
		// call getMethod of hybrid (basic)
		if(!obj->getMethod(key)) luaL_error(L, "Invalid method %s", key);
		return 1;
	}
	
	int HybridBase::class__index(lua_State *L)
	{
		SLB_DEBUG_STACK(6, L, "Call class__index");
		// trying to traverse the class... create a new InternalHybridSubclass
		ClassInfo *ci = Manager::getInstance().getClass(L,1);
		if (ci == 0) luaL_error(L, "Expected a valid class.");
		luaL_checkstring(L,2); // only valid with strings
		if (ci->hasConstructor())
		{
			ref_ptr<InternalHybridSubclass> subc = new InternalHybridSubclass(ci);
			subc->push(L);

			// -- set cache...
			lua_pushvalue(L,2);  // [+1] key
			lua_pushvalue(L,-2); // [+1] copy of new InternalHybrid...
			ci->setCache(L);     // [-2] keep a copy in the cache
			// -- set cache done

			return 1;
		}
		else
		{
			luaL_error(L, "Hybrid Class(%s) doesn't have constructor."
				" You can not subclass(%s) from it", ci->getName().c_str(),
				lua_tostring(L,2));
			return 0;
		}
	}
}
