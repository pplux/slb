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

#include <SLB/FuncCall.hpp>
#include <SLB/Debug.hpp>

namespace SLB {

	FuncCall::FuncCall() : _Treturn(0)
	{
		SLB_DEBUG(10, "Create FuncCall (%p)",this);
	}

	FuncCall::~FuncCall()
	{
		SLB_DEBUG(10, "Delete FuncCall (%p)",this);
	}

	
	void FuncCall::pushImplementation(lua_State *L)
	{
		lua_pushlightuserdata(L, (FuncCall*) this);
		lua_pushcclosure(L,FuncCall::_call, 1);
	}
	
	int FuncCall::_call(lua_State *L)
	{
		FuncCall *fc = (FuncCall*) lua_touserdata(L,lua_upvalueindex(1));
		return fc->call(L);
	}

}
