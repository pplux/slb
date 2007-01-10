#ifndef __SLB_PUSH_GET__
#define __SLB_PUSH_GET__

#include "lua.hpp"

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
	Private::Type<T>::push(L,v);
}

// get function based on Private::Type<class>
template<class T>
inline T get(lua_State *L, int pos)
{
	return Private::Type<T>::get(L,pos);
}

template<class T>
inline bool check(lua_State *L, int pos)
{
	return Private::Type<T>::check(L,pos);
}
template<class T> 
inline void setGlobal(lua_State *L, T v, const char *name)
{
	SLB::push(L,v);
	lua_setglobal(L,name);
}

template<class T>
inline T getGlobal(lua_State *L, const char *name)
{
	lua_getglobal(L,name);
	T value = SLB::get<T>(L, -1);
	lua_pop(L,1); // remove the value
	return value;
}

} // end of SLB namespace

#endif
