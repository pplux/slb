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

#ifndef __SLB_ITERATOR__
#define __SLB_ITERATOR__

#include "Export.hpp"
#include "Object.hpp"
#include "PushGet.hpp"

struct lua_State;

namespace SLB
{

	class SLB_EXPORT IteratorBase
	{
	public:
		virtual int push(lua_State *L) = 0;
		virtual ~IteratorBase() {}
	};

	template<class T, class T_iterator>
	class SLB_EXPORT StdIterator : public IteratorBase
	{
	public:
		typedef T_iterator (T::*GetIteratorMember)();
		StdIterator(GetIteratorMember m_first, GetIteratorMember m_end );
		int push(lua_State *L);
	protected:
		static int next(lua_State *L) ;
	private:
		GetIteratorMember _begin, _end;

		StdIterator( const StdIterator &slbo);
		StdIterator& operator=( const StdIterator &slbo);
	};

	class SLB_EXPORT Iterator : public Object
	{
	public: 
		Iterator(IteratorBase *b);

	protected:
		void pushImplementation(lua_State *L);
		virtual ~Iterator();
	private:
		static int iterator_call(lua_State *L);
		IteratorBase *_iterator;
		Iterator( const Iterator &slbo);
		Iterator& operator=( const Iterator &slbo);
	};

	// ------------------------------------------------------------
	// ------------------------------------------------------------
	// ------------------------------------------------------------
	
	template<class T, class T_iterator>
	inline StdIterator<T, T_iterator>::StdIterator(GetIteratorMember m_first, GetIteratorMember m_end)
		: _begin(m_first), _end(m_end)
	{
	}
	
	template<class T, class T_iterator>
	inline int StdIterator<T, T_iterator>::push(lua_State *L)
	{
		T* container = SLB::get<T*>(L,1);
		lua_pushcclosure(L, StdIterator<T, T_iterator>::next, 0);
		T_iterator *d = reinterpret_cast<T_iterator*>(lua_newuserdata(L, sizeof(T_iterator)*2));
		d[0] = (container->*_begin)();
		d[1] = (container->*_end)();
		return 2;
	}

	template<class T, class T_iterator>
	inline int StdIterator<T, T_iterator>::next(lua_State *L)
	{
		T_iterator *d = reinterpret_cast<T_iterator*>(lua_touserdata(L,1));
		
		if ( d[0] != d[1] )
		{
			SLB::push(L, *(d[0]) );
			d[0]++; // inc iterator
		}
		else
		{
			lua_pushnil(L);
		}
		return 1;
	}

} //end of SLB namespace

#endif
