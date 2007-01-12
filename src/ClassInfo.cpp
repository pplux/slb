#include <SLB/ClassInfo.hpp>
#include <SLB/Manager.hpp>
#include <SLB/Debug.hpp>

namespace SLB {

	class Instance 
	{
	public:
		Instance(ClassInfo *c, void *obj) : _class(c), _const(false), _callGC(true), _obj(obj) {}
		Instance(ClassInfo *c, const void *obj) : _class(c), _const(true), _callGC(false), _const_obj(obj) {}
		void destroy(lua_State *L);

		bool isConst() const { return _const; }
		void *getObj() { return _obj; }
		const void* getObj() const { return _const_obj; }
		ClassInfo *getClass() { return _class.get(); }

		void setGC(bool b) { _callGC = b; }

	protected:
		~Instance() {}
		ref_ptr<ClassInfo> _class;
		bool _const;
		bool _callGC;
		union {
			void *_obj;
			const void *_const_obj;
		};
	};

	void Instance::destroy( lua_State *L )
	{
		if (!_const && _obj )
		{
			if (_callGC && _class->_onGarbageCollection)
			{
				_class->_onGarbageCollection( _obj, L);
			}
		}
		delete this;
	}


	ClassInfo::ClassInfo(const std::type_info &ti) : 
		_typeid(0), _name(""), _onPushCallback(0), _onGarbageCollection(0)
	{
		_typeid = &ti;
		Manager::getInstance().addClass(this);
		setName(_typeid->name());
	}

	ClassInfo::~ClassInfo()
	{
	}
	
	bool ClassInfo::checkInstance(lua_State *L, const void *obj)
	{
		int top = lua_gettop(L);

		// get metatable:
		push(L);
		lua_getmetatable(L,-1);

		lua_getfield(L, -1, "__objects");
		lua_pushlightuserdata(L, (void*)obj);
		lua_rawget(L, -2);
		if (lua_isnil(L,-1))
		{
			lua_settop(L, top); 
			return false;
		}
		lua_replace(L, top+1);
		lua_settop(L, top+1);
		return true;
	}
	
	void ClassInfo::pushRawInstance(lua_State *L, Instance *instance)
	{
		int top = lua_gettop(L);
		
		Instance **obj = reinterpret_cast<Instance**>
			(lua_newuserdata(L, sizeof(Instance*))); // top+1
		*obj = instance;

		// get metatable (class table's metatable)
		push(L);  // (table) top+2
		lua_getmetatable(L,-1);
		lua_replace(L,-2);

		lua_pushvalue(L,-1);
		lua_setmetatable(L, top+1);

		// keep a copy
		lua_getfield(L, top+2, "__objects");
		lua_pushlightuserdata(L, instance->getObj());
		lua_pushvalue(L,top+1);
		lua_rawset(L, -3);

		lua_settop(L, top+1);

		if (!instance->isConst() && _onPushCallback) 
		{
			_onPushCallback(instance->getObj(), L);
		}
	}

	void ClassInfo::pushInstance(lua_State *L, void *obj, bool callGC)
	{
		if (!checkInstance(L, obj))
		{
			Instance *instance = new Instance(this, obj); 
			instance->setGC(callGC);
			pushRawInstance(L, instance);
		}
	}

	void ClassInfo::pushInstance(lua_State *L, const void *obj)
	{
		if (!checkInstance(L, obj))
		{
			pushRawInstance(L, new Instance(this, obj));
		}
	}
	
	void *ClassInfo::convertFrom(void *obj, const ClassInfo *derived_class) const
	{
		if ( derived_class == this )
		{
			return obj;
		}
		else 
		{
			// look for base classes
			for( BaseClassMap::const_iterator 
				i  = derived_class->_baseClasses.begin();
				i != derived_class->_baseClasses.end();
				++i)
			{
				// make the conversion
				if ( i->first.get() == this )
				{
					void *nobj = i->second( obj );
					SLB_DEBUG(5, "Conversion from %s(%p) -> %s(%p)",
						derived_class->_name.c_str(), obj,
						_name.c_str(), nobj);
					return nobj;
				}
			}
		}
		SLB_DEBUG(0, "Can not convert from %s -> %s",
			derived_class->_name.c_str(), _name.c_str());
		return 0;
	}

	void *ClassInfo::getInstance(lua_State *L, int pos) const
	{
		void *obj = 0;
		Instance *i = getRawInstance(L, pos);
		if (i && !i->isConst())
		{
			obj = convertFrom(i->getObj(), i ->getClass());
		}
		return obj;
	}

	const void* ClassInfo::getConstInstance(lua_State *L, int pos) const
	{
		void *obj = 0;
		Instance *i = getRawInstance(L, pos);
		if (i)
		{
			obj = convertFrom(i->getObj(), i ->getClass());
		}
		return obj;
	}
	
	Instance* ClassInfo::getRawInstance(lua_State *L, int pos) const
	{
		Instance *instance = 0;
		int top = lua_gettop(L);
		if (lua_getmetatable(L, pos))
		{
			lua_getfield(L, -1, "__class_ptr");
			if (!lua_isnil(L,-1))
			{
				instance = 
					*reinterpret_cast<Instance**>(lua_touserdata(L, pos));
			}
		}
		lua_settop(L, top);
		return instance;
	}
	
	int ClassInfo::get(lua_State *L, const std::string &key)
	{
		Object *obj = Table::get(key);

		if (obj == 0)
		{
			// look in base classes
			for(BaseClassMap::iterator i = _baseClasses.begin(); 
				i != _baseClasses.end() && obj == 0; ++i)
			{
				obj = ((Table*)i->first.get())->get(key);
			}
		}

		if (obj == 0)
		{
			luaL_error(L, "ClassInfo %s doesn't have %s",_name.c_str(), key.c_str());
			return 0;
		}
		else
		{
			obj->push(L);
			return 1;
		}
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
	
	void ClassInfo::inheritsFrom(const std::type_info &base, ConvertToBase func)
	{
		ClassInfo *c = Manager::getInstance().getOrCreateClass(base);
		_baseClasses[ c ] = func;
	}

	void ClassInfo::setName(const std::string& name)
	{
		_name = name;
		Manager::getInstance().setName(_name, _typeid);
	}
	
	void ClassInfo::setConstructor( FuncCall *constructor )
	{
		_constructor = constructor;
	}
	
	int ClassInfo::__gc(lua_State *L)
	{
		Instance* instance = 
			*reinterpret_cast<Instance**>(lua_touserdata(L, 1));
		SLB_DEBUG(6, "L(%p) GC of instance %p(%s)", L, instance->getObj(), instance->getClass()->getName().c_str());
		instance->destroy(L); 
		return 0;
	}

	int ClassInfo::__call(lua_State *L)
	{
		luaL_error(L, "ClassInfo '%s' is abstract ( or doesn't have a constructor ) ", _name.c_str());
		return 0;
	}
	
}
