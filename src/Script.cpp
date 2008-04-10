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

#include<SLB/Script.hpp>

namespace SLB {

	Script::Script(bool default_libs) : L(0)
	{
		L = luaL_newstate();
		assert("Can not create more lua_states" && (L != 0L));
		if (default_libs) luaL_openlibs(L);
		SLB::Manager::getInstance().registerSLB(L);
	}

	Script::~Script()
	{
		lua_close(L);
		L = 0;
	}

	void Script::doFile(const std::string &filename)
	{
		if ( luaL_dofile(L, filename.c_str()) )
		{
			throw std::runtime_error( lua_tostring(L,-1) );
		}
	}

	void Script::doString(const std::string &code)
	{
		if ( luaL_dostring(L, code.c_str()) )
		{
			throw std::runtime_error( lua_tostring(L,-1) );
		}
	}

} /* SLB */
