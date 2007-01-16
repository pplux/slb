#include <SLB/ClassInfo.hpp>
#include <SLB/Manager.hpp>
#include <SLB/Debug.hpp>

namespace SLB {

	ClassInfo::ClassInfo(const std::type_info &ti) : _typeid(0), _name(""), _instanceFactory(0)
	{
		_typeid = &ti;
		Manager::getInstance().addClass(this);
		_name = _typeid->name();
	}

	ClassInfo::~ClassInfo()
	{
		delete _instanceFactory; 
	}

	void ClassInfo::setName(const std::string& name)
	{
		// rename first in the manager...
		Manager::getInstance().rename(this, name);
		_name = name;
	}
	
	void ClassInfo::setInstanceFactory( InstanceFactory *factory)
	{
		delete _instanceFactory; // delete old..
		_instanceFactory = factory;
	}

	void ClassInfo::setConstructor( FuncCall *constructor )
	{
		_constructor = constructor;
	}

	void ClassInfo::pushImplementation(lua_State *L)
	{
		Table::pushImplementation(L);
		lua_getmetatable(L, -1);
		
		lua_pushstring(L, "__objects");
		lua_newtable(L);
		lua_rawset(L, -3);
		
		lua_pushstring(L, "__class_ptr");
		lua_pushlightuserdata(L, (void*)this);
		lua_rawset(L, -3);

		if ( _constructor.valid() )
		{
			lua_pushstring(L, "__call");
			_constructor->push(L);
			lua_rawset(L, -3);
		}

		lua_pop(L,1); // remove metatable
	}

	void ClassInfo::pushInstance(lua_State *L, InstanceBase *instance)
	{
		int top = lua_gettop(L);
		if (instance == 0)
		{
			SLB_DEBUG(7, "Push(%s) Invalid!!", _name.c_str());
			lua_pushnil(L);
			return;
		}
		
		InstanceBase **obj = reinterpret_cast<InstanceBase**>
			(lua_newuserdata(L, sizeof(InstanceBase*))); // top+1
		*obj = instance;

		SLB_DEBUG(7, "Push(%s) InstanceHandler(%p) -> ptr(%p) const_ptr(%p)",
				_name.c_str(), instance, instance->get_ptr(), instance->get_const_ptr());

		// get metatable (class table's metatable)
		push(L);  // (table) top+2
		lua_getmetatable(L,-1);
		lua_replace(L,-2);

		lua_pushvalue(L,-1);
		lua_setmetatable(L, top+1);

		// keep a copy
		lua_getfield(L, top+2, "__objects");
		lua_pushlightuserdata(L, const_cast<void*>(instance->get_const_ptr()) );
		lua_pushvalue(L,top+1);
		lua_rawset(L, -3);

		lua_settop(L, top+1);
	}

	void *ClassInfo::get_ptr(lua_State *L, int pos)
	{
		void *obj = 0;
		InstanceBase *i = getInstance(L, pos);
		if (i)
		{
			obj = Manager::getInstance().convert( i->getClass()->getTypeid(), getTypeid(), i->get_ptr() );
		}
		SLB_DEBUG(7, "Class(%s) get_ptr -> %p", _name.c_str(), obj);
		return obj;
	}

	const void* ClassInfo::get_const_ptr(lua_State *L, int pos)
	{
		const void *obj = 0;
		InstanceBase *i = getInstance(L, pos);
		if (i)
		{
			obj = Manager::getInstance().convert( i->getClass()->getTypeid(), getTypeid(), i->get_const_ptr() );
		}
		SLB_DEBUG(7, "Class(%s) get_const_ptr -> %p", _name.c_str(), obj);
		return obj;
	}

	InstanceBase* ClassInfo::getInstance(lua_State *L, int pos) const
	{
		InstanceBase *instance = 0;
		int top = lua_gettop(L);
		if (lua_getmetatable(L, pos))
		{
			lua_getfield(L, -1, "__class_ptr");
			if (!lua_isnil(L,-1))
			{
				instance = 
					*reinterpret_cast<InstanceBase**>(lua_touserdata(L, pos));
			}
		}
		lua_settop(L, top);
		return instance;
	}

	void ClassInfo::push_ref(lua_State *L, void *ref )
	{
		if (_instanceFactory)
		{
			pushInstance(L, _instanceFactory->create_ref(ref) );
			SLB_DEBUG(7, "Class(%s) push_ref -> %p", _name.c_str(), ref);
		}
		else
		{
			luaL_error(L, "Can not push a ref of class %s", _name.c_str());
		}
	}

	void ClassInfo::push_ptr(lua_State *L, void *ptr, bool fromConstructor)
	{
		if (_instanceFactory)
		{
			pushInstance(L, _instanceFactory->create_ptr(ptr, fromConstructor) );
			SLB_DEBUG(7, "Class(%s) push_ptr (from_Constructor %d) -> %p", _name.c_str(), fromConstructor, ptr);
		}
		else
		{
			luaL_error(L, "Can not push a ptr of class %s", _name.c_str());
		}
	}

	void ClassInfo::push_const_ptr(lua_State *L, const void *const_ptr)
	{
		if (_instanceFactory)
		{
			pushInstance(L, _instanceFactory->create_const_ptr(const_ptr) );
			SLB_DEBUG(7, "Class(%s) push const_ptr -> %p", _name.c_str(), const_ptr);
		}
		else
		{
			luaL_error(L, "Can not push a const_ptr of class %s", _name.c_str());
		}
	}

	void ClassInfo::push_copy(lua_State *L, const void *ptr)
	{
		if (_instanceFactory)
		{
			pushInstance(L, _instanceFactory->create_copy(ptr) );
			SLB_DEBUG(7, "Class(%s) push copy -> %p", _name.c_str(), ptr);
		}
		else
		{
			luaL_error(L, "Can not push a copy of class %s", _name.c_str());
		}
	}
	
	
	int ClassInfo::__gc(lua_State *L)
	{
		InstanceBase* instance = 
			*reinterpret_cast<InstanceBase**>(lua_touserdata(L, 1));
		delete instance; 
		return 0;
	}

	int ClassInfo::__call(lua_State *L)
	{
		luaL_error(L, "ClassInfo '%s' is abstract ( or doesn't have a constructor ) ", _name.c_str());
		return 0;
	}

}
