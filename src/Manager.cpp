#include <SLB/Manager.hpp>
#include <SLB/ClassInfo.hpp>
#include <SLB/lua.hpp>
#include <SLB/Debug.hpp>

namespace SLB {

	ref_ptr<Manager> Manager::_instance = 0;

	Manager::Manager()
	{
		SLB_DEBUG(0, "Manager initialization");
	}

	Manager::~Manager()
	{
		SLB_DEBUG(0, "Manager destruction");
	}
	
	void Manager::pushImplementation(lua_State *L)
	{
		lua_newtable(L);
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
	
	void Manager::setName( const std::string &id, const std::type_info *ti)
	{
		_names[id] = ti;
	}
}

