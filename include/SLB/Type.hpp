#ifndef __SLB_TYPE__
#define __SLB_TYPE__

#include "lua.hpp"
#include "Debug.hpp"
#include "SPP.hpp"
#include "Manager.hpp"
#include "ClassInfo.hpp"


namespace SLB {
namespace Private {

	// Default implementation
	template<class T>
	struct Type
	{
		static ClassInfo *getClass(lua_State *L)
		{
			ClassInfo *c = SLB::Manager::getInstance().getClass(typeid(T));
			if (c == 0) luaL_error(L, "Unknown class %s", typeid(T).name());
			return c;
		}

		static void push(lua_State *L,const T &obj)
		{
			getClass(L)->push_copy(L, (void*) &obj);
		}

		static T get(lua_State *L, int pos)
		{
			return *( reinterpret_cast<T*>( getClass(L)->get_ptr(L, pos) ));
		}

		static bool check(lua_State *L, int pos)
		{
			return getClass(L)->check(L, pos);
		}
		
	};

	template<class T>
	struct Type<T*>
	{
		static ClassInfo *getClass(lua_State *L)
		{
			ClassInfo *c = SLB::Manager::getInstance().getClass(typeid(T));
			if (c == 0) luaL_error(L, "Unknown class %s", typeid(T).name());
			return c;
		}

		static void push(lua_State *L, T *obj, bool fromConstructor = false)
		{
			ClassInfo *c = SLB::Manager::getInstance().getClass(typeid(*obj));
			if (c) 
			{
				c->push_ptr(L, dynamic_cast<void*>(obj), fromConstructor);
			}
			else
			{
				if (fromConstructor)
				{
					luaL_error(L, "Can't push from constructor of obj(%s) and use a Base Class (%s)",
						typeid(*obj).name(), typeid(T).name());
				}
				getClass(L)->push_ptr(L, (void*) obj);
			}
		}

		static T* get(lua_State *L, int pos)
		{
			return reinterpret_cast<T*>( getClass(L)->get_ptr(L, pos) );
		}

		static bool check(lua_State *L, int pos)
		{
			return getClass(L)->check(L, pos);
		}
	};
	
	template<class T>
	struct Type<const T*>
	{
		static ClassInfo *getClass(lua_State *L)
		{
			ClassInfo *c = SLB::Manager::getInstance().getClass(typeid(T));
			if (c == 0) luaL_error(L, "Unknown class %s", typeid(T).name());
			return c;
		}

		static void push(lua_State *L,const T *obj)
		{
			ClassInfo *c = SLB::Manager::getInstance().getClass(typeid(*obj));
			if (c) 
			{
				c->push_const_ptr(L, dynamic_cast<void*>(obj));
			}
			else
			{
				getClass(L)->push_const_ptr(L, (void*) obj);
			}
		}

		static const T* get(lua_State *L, int pos)
		{
			return reinterpret_cast<const T*>( getClass(L)->get_const_ptr(L, pos) );
		}

		static bool check(lua_State *L, int pos)
		{
			return getClass(L)->check(L, pos);
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
		static ClassInfo *getClass(lua_State *L)
		{
			ClassInfo *c = SLB::Manager::getInstance().getClass(typeid(T));
			if (c == 0) luaL_error(L, "Unknown class %s", typeid(T).name());
			return c;
		}

		static void push(lua_State *L,T &obj)
		{
			getClass(L)->push_ref(L, (void*) obj);
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
