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
			if (obj == 0)
			{
				lua_pushnil(L);
				return;
			}
			if (typeid(*obj) != typeid(T))
			{
				// check if the internal class exists...
				ClassInfo *c = SLB::Manager::getInstance().getClass(typeid(*obj));
				if ( c ) 
				{
					// covert the object to the internal class...
					void *real_obj = SLB::Manager::getInstance().convert( &typeid(T), &typeid(*obj), obj );
					c->push_ptr(L, real_obj, fromConstructor);
					return;
				}
			}
			// use this class...	
			getClass(L)->push_ptr(L, (void*) obj, fromConstructor);

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
			if (obj == 0)
			{
				lua_pushnil(L);
				return;
			}
			if (typeid(*obj) != typeid(T))
			{
				// check if the internal class exists...
				ClassInfo *c = SLB::Manager::getInstance().getClass(typeid(*obj));
				if ( c ) 
				{
					// covert the object to the internal class...
					const void *real_obj = SLB::Manager::getInstance().convert( &typeid(T), &typeid(*obj), obj );
					c->push_const_ptr(L, real_obj);
					return;
				}
			}
			getClass(L)->push_const_ptr(L, (const void*) obj);
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
			const T* obj = Type<const T*>::get(L,pos);
			//TODO: remove the typeid(T).getName() and use classInfo :)
			if (obj == 0L) luaL_error(L, "Can not get a reference of class %s", typeid(T).name());
			return *(obj);
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
			getClass(L)->push_ref(L, (void*) &obj);
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

	template<>
	struct Type<unsigned long>
	{
		static void push(lua_State *L, unsigned long v)
		{
			SLB_DEBUG(6, "Push unsigned long = %f",v);
			lua_pushnumber(L,v);
		}

		static unsigned long get(lua_State *L, int p)
		{
			unsigned long v = (unsigned long) lua_tonumber(L,p);
			SLB_DEBUG(6,"Get unsigned long (pos %d) = %f",p,v);
			return v;
		}

		static bool check(lua_State *L, int pos)
		{
			return (lua_isnumber(L,pos) != 0);
		}
	};

	template<>
	struct Type<const unsigned long&>
	{
		static void push(lua_State *L, const unsigned long &v)
		{
			Type<unsigned long>::push(L,v);
		}

		static unsigned long get(lua_State *L, int p)
		{
			return Type<unsigned long>::get(L,p);
		}

		static bool check(lua_State *L, int pos)
		{
			return Type<unsigned long>::check(L,pos);
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

	template<>
	struct Type<const unsigned char*>
	{
		static void push(lua_State *L, const unsigned char* v)
		{
			SLB_DEBUG(6, "Push const unsigned char* = %s",v);
			lua_pushstring(L,(const char*)v);
		}

		static const unsigned char* get(lua_State *L, int p)
		{
			const unsigned char* v = (const unsigned char*) lua_tostring(L,p);
			SLB_DEBUG(6,"Get const unsigned char* (pos %d) = %s",p,v);
			return v;
		}

		static bool check(lua_State *L, int pos)
		{
			return (lua_isstring(L,pos) != 0);
		}
	};

}} // end of SLB::Private

#endif
