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


	// Standard iterator
	template<class T, class T_iterator>
	struct StdIteratorTraits
	{
		typedef T Container;
		typedef T_iterator Iterator;
		typedef Iterator (Container::*GetIteratorMember)();
	};

	template<class T, class T_iterator>
	struct StdConstIteratorTraits
	{
		typedef T Container;
		typedef T_iterator Iterator;
		typedef Iterator (Container::*GetIteratorMember)() const;
	};

	template<typename Traits>
	class StdIterator : public IteratorBase
	{
	public:
		typedef typename Traits::GetIteratorMember MemberFuncs ;
		typedef typename Traits::Container Container;
		typedef typename Traits::Iterator  Iterator;

		StdIterator(MemberFuncs m_first, MemberFuncs m_end );
		int push(lua_State *L);
	protected:
		static int next(lua_State *L) ;
	private:
		MemberFuncs _begin, _end;

		StdIterator( const StdIterator &slbo);
		StdIterator& operator=( const StdIterator &slbo);
	};

	// ------------------------------------------------------------
	// ------------------------------------------------------------
	// ------------------------------------------------------------
	
	template<class T>
	inline StdIterator<T>::StdIterator(MemberFuncs m_first, MemberFuncs m_end)
		: _begin(m_first), _end(m_end)
	{
	}
	
	template<class T>
	inline int StdIterator<T>::push(lua_State *L)
	{
		Container* container = SLB::get<Container*>(L,1);
		lua_pushcclosure(L, StdIterator<T>::next, 0);
		Iterator *d = reinterpret_cast<Iterator*>(lua_newuserdata(L, sizeof(Iterator)*2));
		d[0] = (container->*_begin)();
		d[1] = (container->*_end)();
		return 2;
	}

	template<class T>
	inline int StdIterator<T>::next(lua_State *L)
	{
		Iterator *d = reinterpret_cast<Iterator*>(lua_touserdata(L,1));
		
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
