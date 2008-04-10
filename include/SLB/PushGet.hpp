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

#ifndef __SLB_PUSH_GET__
#define __SLB_PUSH_GET__

#include "lua.hpp"
#include "Debug.hpp"

// Fixed Apple problems
#ifdef check
    #undef check
#endif

namespace SLB {

//----------------------------------------------------------------------------
//-- Push/Get/Check functions ------------------------------------------------
//----------------------------------------------------------------------------
	template<class T> void push(lua_State *L, T v);
	template<class T> T get(lua_State *L, int pos);
	template<class T> bool check(lua_State *L, int pos);
	template<class T> void setGlobal(lua_State *L, T v, const char *name);
	template<class T> T getGlobal(lua_State *L, const char *name);
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

namespace Private {
	template<class C>
	struct Type;
}

template<class T>
inline void push(lua_State *L, T v)
{
	SLB_DEBUG_CALL; 
	Private::Type<T>::push(L,v);
}

// get function based on Private::Type<class>
template<class T>
inline T get(lua_State *L, int pos)
{
	SLB_DEBUG_CALL; 
	return Private::Type<T>::get(L,pos);
}

template<class T>
inline bool check(lua_State *L, int pos)
{
	SLB_DEBUG_CALL; 
	return Private::Type<T>::check(L,pos);
}
template<class T> 
inline void setGlobal(lua_State *L, T v, const char *name)
{
	SLB_DEBUG_CALL; 
	SLB::push(L,v);
	lua_setglobal(L,name);
}

template<class T>
inline T getGlobal(lua_State *L, const char *name)
{
	SLB_DEBUG_CALL; 
	lua_getglobal(L,name);
	T value = SLB::get<T>(L, -1);
	lua_pop(L,1); // remove the value
	return value;
}

} // end of SLB namespace

#endif
