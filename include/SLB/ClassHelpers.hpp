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

#ifndef __SLB_CLASS_HELPERS__
#define __SLB_CLASS_HELPERS__

#include "SPP.hpp"
#include "FuncCall.hpp"

struct lua_State;

namespace SLB {

	template<class C>
	struct Operator
	{
		static C* defaultAdd (const C *a,  const C *b)  { return new C( (*a)+(*b) ); }	
		static C* defaultSub (const C *a,  const C *b)  { return new C( (*a)-(*b) ); }	
		static C* defaultMult(const C *a, const C *b)   { return new C( (*a)*(*b) ); }	
		static C* defaultDiv (const C *a,  const C *b)  { return new C( (*a)/(*b) ); }	
	};

}


#endif
