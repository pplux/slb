#include <SLB/Manager.hpp>
#include <SLB/ClassInfo.hpp>
#include <SLB/lua.hpp>
#include <SLB/Debug.hpp>


namespace SLB {

	/* Global functions */
	int SLB_type(lua_State *L)
	{
		int top = lua_gettop(L);
		lua_getmetatable(L,1);
		if (!lua_isnil(L,-1))
		{
			lua_getfield(L, -1, "__class_ptr");
			if (!lua_isnil(L,-1))
			{
				ClassInfo* ci = reinterpret_cast<ClassInfo*>( lua_touserdata(L,-1) );
				lua_settop(L, top);
				lua_pushstring(L, ci->getName().c_str());
				return 1;
			}
		}
		lua_settop(L, top);
		return 0;
	}

	static const luaL_Reg SLB_funcs[] = {
		{"SLB_type", SLB_type},
		{NULL, NULL}
	};

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

		//check if SLB is already registered...
		lua_getfield(L, LUA_REGISTRYINDEX, "SLB");
		if (lua_isnil(L,-1))
		{
			lua_pop(L,1);
			lua_pushboolean(L,1);
			lua_setfield(L,LUA_REGISTRYINDEX, "SLB");
		}
		else
		{
			lua_pop(L,1);
			return;
		}

		// use _G metatable for our values...
		lua_getmetatable(L, LUA_GLOBALSINDEX);
		if (lua_isnil(L, -1))
		{
			lua_newtable(L);
		}
		else
		{
			SLB_DEBUG(0, "WARNING: Lua_State %p has a global metatable...", L);
		}
		lua_pushstring(L,"__index");
		_global->push(L);
		lua_rawset(L,-3);
		lua_setmetatable(L, LUA_GLOBALSINDEX);

		lua_settop(L,top);

		// Register global functions
		lua_pushvalue(L, LUA_GLOBALSINDEX);
		luaL_register(L, 0, SLB_funcs);
		lua_pop(L,1);
	}
	
	Manager *Manager::getInstancePtr()
	{
		static Manager _instance;
		return &_instance;
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

