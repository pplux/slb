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
#include<SLB/Debug.hpp>
#include<stdexcept>
#include<sstream>

namespace SLB {

#if SLB_DEBUG_LEVEL != 0
	/* Debugging function, see TODO on Script::getState */
	void ScriptHook(lua_State *L, lua_Debug *ar)
	{
		lua_getinfo(L, "Sl",ar);
		__SLB_ADJUST__();
		SLB_DEBUG_FUNC("SLB-X[%p] %s:%d",L,ar->short_src,ar->currentline );
		SLB_DEBUG_FUNC("\n");\
	}
#endif

	Script::Script(bool default_libs) : _L(0), _loadDefaultLibs(default_libs)
	{
		SLB_DEBUG_CALL;
	}

	Script::~Script()
	{
		SLB_DEBUG_CALL;
		close();
	}
	
	lua_State* Script::getState()
	{
		SLB_DEBUG_CALL;
		if (!_L)
		{
			SLB_DEBUG(10, "Open default libs = %s", _loadDefaultLibs ? " true": " false");
			_L = luaL_newstate();
			assert("Can not create more lua_states" && (_L != 0L));
			if (_loadDefaultLibs) luaL_openlibs(_L);
			SLB::Manager::getInstance().registerSLB(_L);
		
			//TODO: Promote that functionality to a higher interface to allow proper
			//      debugging
			//
			/* if debug_level > 0 ........ */
			#if SLB_DEBUG_LEVEL != 0
			lua_sethook(_L, ScriptHook, LUA_MASKLINE, 0);
			#endif
			/* end debug */
		}
		return _L;
	}

	void Script::close()
	{
		SLB_DEBUG_CALL;
		if (_L)
		{
			lua_close(_L);
			_L = 0;
		}
	}

	void Script::callGC()
	{
		SLB_DEBUG_CALL;
		if (_L)
		{
			lua_gc(_L, LUA_GCCOLLECT, 0);
		}
	}

	void Script::doFile(const std::string &filename)
	{
		SLB_DEBUG_CALL;
		lua_State *L = getState();
		SLB_DEBUG(10, "filename %s = ", filename.c_str());
		if ( luaL_dofile(L, filename.c_str()) )
		{
			throw std::runtime_error( lua_tostring(L,-1) );
		}
	}

	void Script::doString(const std::string &o_code, const std::string &hint)
	{
		SLB_DEBUG_CALL;
		lua_State *L = getState();
		SLB_DEBUG(10, "code = %10s, hint = %s", o_code.c_str(), hint.c_str()); 
		std::stringstream code;
		code << "--" << hint << std::endl << o_code;

		if ( luaL_dostring(L, code.str().c_str()) )
		{
			throw std::runtime_error( lua_tostring(L,-1) );
		}
	}

} /* SLB */
