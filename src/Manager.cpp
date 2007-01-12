#include <SLB/Manager.hpp>
#include <SLB/ClassInfo.hpp>
#include <SLB/lua.hpp>
#include <SLB/Debug.hpp>

namespace SLB {

	ref_ptr<Manager> Manager::_instance = 0;

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
		push(L);
		lua_rawset(L,-3);
		lua_setmetatable(L, LUA_GLOBALSINDEX);

		lua_settop(L,top);
	}
	
	void Manager::pushImplementation(lua_State *L)
	{
		_global->push(L); // top +1
		
	}

	Manager *Manager::getInstancePtr()
	{
		if (!_instance.valid())
		{
			_instance = new Manager();
		}
		return _instance.get();
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

	ClassInfo *Manager::getClass(const std::type_info &ti)
	{
		ClassMap::iterator i = _classes.find(&ti);
		if ( i != _classes.end() ) return i->second.get();
		return 0;
	}

	ClassInfo *Manager::getClass(const std::string &name)
	{
		NameMap::iterator i = _names.find(name);
		if ( i != _names.end() ) return getClass( *i->second );
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
	
	void Manager::setName( const std::string &old, const std::string &new_name, const std::type_info *ti)
	{
		if (old != "")
		{
			_names.erase(old);
			_global->erase(old);
		}
		if (new_name != "")
		{
			_names[new_name] = ti;
			_global->set(new_name, getOrCreateClass(*ti) );
		}
	}
}

