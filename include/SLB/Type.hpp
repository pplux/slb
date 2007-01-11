#ifndef __SLB_TYPE__
#define __SLB_TYPE__

#include "lua.hpp"
#include "Debug.hpp"
#include "SPP.hpp"
#include "Manager.hpp"
#include "Class.hpp"


namespace SLB {
namespace Private {

	// Default implementation
	template<class T>
	struct Type
	{
		static void push(lua_State *L,const T &obj)
		{
			T *data = (T*) lua_newuserdata(L, sizeof(T));
			SLB_DEBUG(5,
				"Push UNKNOWN Data of type %s (%p)->lua[%p] size = %lu",
				typeid(T).name(),(void*)&obj,(void*) data,
				(long unsigned int)sizeof(T)
			);
			luaL_newmetatable(L, typeid(T).name());
			lua_setmetatable(L,-2);
			*data = obj;
		}

		static T get(lua_State *L, int pos)
		{
			void *data = luaL_checkudata(L,pos, typeid(T).name());
			SLB_DEBUG(5,
				"Get UNKNOWN Data of type %s (%p)", typeid(T).name(), (void*) data
			);
			return *((T*) data);
		}

		static bool check(lua_State *L, int pos)
		{
			int  top = lua_gettop(L);
			bool result = true;
			lua_getmetatable(L,pos);
			if (lua_isnil(L,-1)) result = false;
			else
			{
				luaL_newmetatable(L, typeid(T).name());
				if (lua_equal(L,-1,-2) == 0) result = false;
			}
			lua_settop(L,top);
			return result;
		}
	};

	template<class T>
	struct Type<T*>
	{
		static void push(lua_State *L, T *obj, bool callGC = true)
		{
			if (obj == 0) 
			{
				lua_pushnil(L);
				return;
			}
			Class *c = SLB::Manager::getInstance().getClass(typeid(*obj));
			if (c) 
			{
				SLB_DEBUG(6, "Push obj %p (%s)",obj,c->getName().c_str());
				c->pushInstance(L, dynamic_cast<void*>(obj), callGC );
			}
			else
			{
				// try fallback to the raw-pointer class:
				c = SLB::Manager::getInstance().getClass(typeid(T));
				if (c)
				{
					SLB_DEBUG(6, "Push const-obj %p (%s)",obj,c->getName().c_str());
					c->pushInstance(L, obj, callGC);
				}
				else
				{
					SLB_DEBUG(0,
						"*** ERROR *** Class %s not registerd (invalid push)",
				   		typeid(T).name()
					);
					lua_pushnil(L);
				}
			}
		}

		static T* get(lua_State *L, int pos)
		{
			const Class *c = SLB::Manager::getInstance().getClass(typeid(T));
			if (c) 
			{
				T* obj = static_cast<T*>(c->getInstance(L, pos));
				SLB_DEBUG(6, "Get obj (pos %d) = %p(%s) ",pos,obj,c->getName().c_str());
				if (obj == 0)
				{
					luaL_error(L, "Arg #%d could not be converted to pointer of type %s",
						(pos>0)? pos : pos + lua_gettop(L) +1, c->getName().c_str());
				}
				return obj;
			}
			else
			{
				SLB_DEBUG(1,
					"*** WARNING *** Class %s not registerd (invalid get)",
				   	typeid(T).name()
				);
				return 0;
			}
		}

		static bool check(lua_State *L, int pos)
		{
			const Class *c = SLB::Manager::getInstance().template getClass<T>();
			if (c) 
			{
				T* obj = static_cast<T*>(c->getInstance(L, pos));
				return (obj != 0);
			}
			return false;
		}
	};
	
	template<class T>
	struct Type<const T*>
	{
		static void push(lua_State *L,const T *obj)
		{
			if (obj == 0) 
			{
				lua_pushnil(L);
				return;
			}
			Class *c = SLB::Manager::getInstance().getClass(typeid(*obj));
			if (c) 
			{
				SLB_DEBUG(6, "Push const-obj %p (%s)",obj,c->getName().c_str());
				c->pushInstance(L, obj);
			}
			else
			{
				// try fallback to the raw-pointer class:
				c = SLB::Manager::getInstance().getClass(typeid(T));
				if (c)
				{
					SLB_DEBUG(6, "Push const-obj %p (%s)",obj,c->getName().c_str());
					c->pushInstance(L, obj);
				}
				else
				{
					SLB_DEBUG(0,
						"*** ERROR *** Class %s not registerd (invalid push)",
				   		typeid(T).name()
					);
					lua_pushnil(L);
				}
			}
		}

		static const T* get(lua_State *L, int pos)
		{
			const Class *c = SLB::Manager::getInstance().getClass(typeid(T));
			if (c) 
			{
				const T* obj = static_cast<const T*>(c->getConstInstance(L, pos));
				SLB_DEBUG(6, "Get const-obj (pos %d) = %p (%s)",pos, obj,c->getName().c_str());
				if (obj == 0)
				{
					luaL_error(L, "Arg #%d could not be converted to const-obj %s",
						(pos>0)? pos : pos + lua_gettop(L) +1, c->getName().c_str());
				}
				return obj;
			}
			else
			{
				SLB_DEBUG(1,
					"*** WARNING *** Class %s not registerd (invalid get)",
				   	typeid(T).name()
				);
				return 0;
			}
		}

		static bool check(lua_State *L, int pos)
		{
			const Class *c = SLB::Manager::getInstance().getClass(typeid(T));
			if (c) 
			{
				const T* obj = static_cast<const T*>(c->getConstInstance(L, pos));
				return (obj != 0);
			}
			return false;
		}
	};

	template<class T>
	struct Type<const T&>
	{
		static void push(lua_State *L,const T &obj)
		{
			Type<const T*>::push(L, &obj);
		}

		static const T& get(lua_State *L, int pos)
		{
			return *(Type<const T*>::get(L,pos));
		}

		static bool check(lua_State *L, int pos)
		{
			return Type<const T*>::check(L,pos);
		}
	};
	
	template<class T>
	struct Type<T&>
	{
		static void push(lua_State *L,T &obj)
		{
			Type<T*>::push(L, &obj);
		}

		static T& get(lua_State *L, int pos)
		{
			return *(Type<T*>::get(L,pos));
		}

		static bool check(lua_State *L, int pos)
		{
			return Type<T*>::check(L,pos);
		}
	};

	template<>
	struct Type<void*>
	{
		static void push(lua_State *L,void* obj)
		{
			if (obj == 0) lua_pushnil(L);
			else lua_pushlightuserdata(L, obj);
		}

		static void *get(lua_State *L, int pos)
		{
			if (check(L,pos)) return lua_touserdata(L,pos);
			//TODO: Check here if is an userdata and convert it to void
			return 0;
		}

		static bool check(lua_State *L, int pos)
		{
			return (lua_islightuserdata(L,pos) != 0);
		}
	};

	// Type specialization for <unsigned int>
	template<>
	struct Type<size_t>
	{
		static void push(lua_State *L, size_t v)
		{
			SLB_DEBUG(6, "Push unsigned integer = %d",v);
			lua_pushinteger(L,v);
		}
		static size_t get(lua_State *L, int p)
		{
			size_t v = static_cast<size_t>(lua_tointeger(L,p));
			SLB_DEBUG(6,"Get unsigned integer (pos %d) = %d",p,v);
			return v;
		}

		static bool check(lua_State *L, int pos)
		{
			return (lua_isnumber(L,pos) != 0);
		}
	};

	template<>
	struct Type<const size_t&>
	{
		static void push(lua_State *L, const size_t &v)
		{
			Type<size_t>::push(L,v);
		}

		static size_t get(lua_State *L, int p)
		{
			return Type<size_t>::get(L,p);
		}

		static bool check(lua_State *L, int pos)
		{
			return Type<size_t>::check(L,pos);
		}
	};

	// Type specialization for <int>
	template<>
	struct Type<int>
	{
		static void push(lua_State *L, int v)
		{
			SLB_DEBUG(6, "Push integer = %d",v);
			lua_pushinteger(L,v);
		}
		static int get(lua_State *L, int p)
		{
			int v = (int) lua_tointeger(L,p);
			SLB_DEBUG(6,"Get integer (pos %d) = %d",p,v);
			return v;
		}

		static bool check(lua_State *L, int pos)
		{
			return (lua_isnumber(L,pos) != 0);
		}
	};

	template<>
	struct Type<const int&>
	{
		static void push(lua_State *L, const int &v)
		{
			Type<int>::push(L,v);
		}

		static int get(lua_State *L, int p)
		{
			return Type<int>::get(L,p);
		}

		static bool check(lua_State *L, int pos)
		{
			return Type<int>::check(L,pos);
		}
	};
	
	// Type specialization for <double>
	template<>
	struct Type<double>
	{
		static void push(lua_State *L, double v)
		{
			SLB_DEBUG(6, "Push double = %f",v);
			lua_pushnumber(L,v);
		}
		static double get(lua_State *L, int p)
		{
			double v = (double) lua_tonumber(L,p);
			SLB_DEBUG(6,"Get double (pos %d) = %f",p,v);
			return v;
		}

		static bool check(lua_State *L, int pos)
		{
			return (lua_isnumber(L,pos) != 0);
		}
	};

	template<>
	struct Type<const double&>
	{
		static void push(lua_State *L, const double &v)
		{
			Type<double>::push(L,v);
		}

		static double get(lua_State *L, int p)
		{
			return Type<double>::get(L,p);
		}

		static bool check(lua_State *L, int pos)
		{
			return Type<double>::check(L,pos);
		}
	};

	// Type specialization for <float>
	template<>
	struct Type<float>
	{
		static void push(lua_State *L, float v)
		{
			SLB_DEBUG(6, "Push float = %f",v);
			lua_pushnumber(L,v);
		}

		static float get(lua_State *L, int p)
		{
			float v = (float) lua_tonumber(L,p);
			SLB_DEBUG(6,"Get float (pos %d) = %f",p,v);
			return v;
		}

		static bool check(lua_State *L, int pos)
		{
			return (lua_isnumber(L,pos) != 0);
		}
	};

	template<>
	struct Type<const float&>
	{
		static void push(lua_State *L, const float &v)
		{
			Type<float>::push(L,v);
		}

		static float get(lua_State *L, int p)
		{
			return Type<float>::get(L,p);
		}

		static bool check(lua_State *L, int pos)
		{
			return Type<float>::check(L,pos);
		}
	};

	
	// Type specialization for <bool>
	template<>
	struct Type<bool>
	{
		static void push(lua_State *L, bool v)
		{
			SLB_DEBUG(6, "Push bool = %d",(int)v);
			lua_pushboolean(L,v);
		}
		static bool get(lua_State *L, int p)
		{
			bool v = (lua_toboolean(L,p) != 0);
			SLB_DEBUG(6,"Get bool (pos %d) = %d",p,v);
			return v;
		}

		static bool check(lua_State *L, int pos)
		{
			return lua_isboolean(L,pos);
		}
	};

	template<>
	struct Type<std::string>
	{
		static void push(lua_State *L, const std::string &v)
		{
			SLB_DEBUG(6, "Push const std::string& = %s",v.c_str());
			lua_pushstring(L, v.c_str());
		}

		static std::string get(lua_State *L, int p)
		{
			const char* v = (const char*) lua_tostring(L,p);
			SLB_DEBUG(6,"Get std::string (pos %d) = %s",p,v);
			return v;
		}

		static bool check(lua_State *L, int pos)
		{
			return (lua_isstring(L,pos) != 0);
		}
	};

	template<>
	struct Type<const std::string &>
	{
		static void push(lua_State *L, const std::string &v)
		{
			SLB_DEBUG(6, "Push const std::string& = %s",v.c_str());
			lua_pushstring(L, v.c_str());
		}

		// let the compiler do the conversion...
		static const std::string get(lua_State *L, int p)
		{
			const char* v = (const char*) lua_tostring(L,p);
			SLB_DEBUG(6,"Get std::string (pos %d) = %s",p,v);
			return std::string(v);
		}

		static bool check(lua_State *L, int pos)
		{
			return (lua_isstring(L,pos) != 0);
		}
	};


	// Type specialization for <const char*>
	template<>
	struct Type<const char*>
	{
		static void push(lua_State *L, const char* v)
		{
			SLB_DEBUG(6, "Push const char* = %s",v);
			lua_pushstring(L,v);
		}

		static const char* get(lua_State *L, int p)
		{
			const char* v = (const char*) lua_tostring(L,p);
			SLB_DEBUG(6,"Get const char* (pos %d) = %s",p,v);
			return v;
		}

		static bool check(lua_State *L, int pos)
		{
			return (lua_isstring(L,pos) != 0);
		}
	};

}} // end of SLB::Private

#endif
