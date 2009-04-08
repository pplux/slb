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

#include <SLB/Debug.hpp>
#include <SLB/Error.hpp>
#include <sstream>

namespace SLB {

int ErrorHandler::lua_pcall(lua_State *L, int nargs, int nresults)
{
	SLB_DEBUG_CALL;

	// insert the error handler before the arguments
	int base = lua_gettop(L) - nargs;
	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, _slb_stackHandler, 1);
	lua_insert(L, base);

	// call the function
	int result = ::lua_pcall(L, nargs, nresults, base);

	// remove the error handler
	lua_remove(L, base);

	// return the previous result
	return result;
}

const char *ErrorHandler::SE_name()
{
	if (_L) return _debug.name;
	return NULL;
}

const char *ErrorHandler::SE_nameWhat()
{
	if (_L) return _debug.namewhat;
	return NULL;
}

const char *ErrorHandler::SE_what()
{
	if (_L) return _debug.what;
	return NULL;
}

const char *ErrorHandler::SE_source()
{
	if (_L) return _debug.source;
	return NULL;
}

const char *ErrorHandler::SE_shortSource()
{
	if (_L) return _debug.short_src;
	return NULL;
}

int ErrorHandler::SE_currentLine()
{
	if (_L) return _debug.currentline;
	return -1;
}

int ErrorHandler::SE_numberOfUpvalues()
{
	if (_L) return _debug.nups;
	return -1;
}

int ErrorHandler::SE_lineDefined()
{
	if (_L) return _debug.linedefined;
	return -1;
}

int ErrorHandler::SE_lastLineDefined()
{
	if (_L) return _debug.lastlinedefined;
	return -1;
}

int ErrorHandler::_slb_stackHandler(lua_State *L)
{
	ErrorHandler *EH = reinterpret_cast<ErrorHandler*>(lua_touserdata(L,lua_upvalueindex(1)));
	if (EH) EH->process(L);
	return 1;
}

void ErrorHandler::process(lua_State *L)
{
	_L = L;
	assert("Invalid state" && _L != 0);
	const char *error = lua_tostring(_L, -1);
	begin(error);
	for ( int level = 0; lua_getstack(_L, level, &_debug ); level++)
	{
		if (lua_getinfo(L, "Slnu", &_debug) )
		{
			stackElement(level);	
		}
		else
		{
			// Throw an exception here ?
			assert("[ERROR using Lua DEBUG INTERFACE]" && false);
		}
	}
	const char *msg = end();
	lua_pushstring(_L, msg);
	_L = 0;
}

/*
	int LuaCallBase::errorHandler(lua_State *L)
	{
		SLB_DEBUG_CALL;
		std::ostringstream out; // Use lua pushfstring and so on...
		lua_Debug debug;

		out << "SLB Exception: "
			<< std::endl << "-------------------------------------------------------"
			<< std::endl;
		out << "Lua Error:" << std::endl << "\t" 
			<<  lua_tostring(L, -1) << std::endl
			<< "Traceback:" << std::endl;
		for ( int level = 0; lua_getstack(L, level, &debug ); level++)
		{
			if (lua_getinfo(L, "Sln", &debug) )
			{
				//TODO use debug.name and debug.namewhat
				//make this more friendly
				out << "\t [ " << level << " (" << debug.what << ") ] ";
				if (debug.currentline > 0 )
				{
					out << debug.short_src << ":" << debug.currentline; 
				}
				if (debug.name)
				{
					out << " @ " << debug.name;
				   if (debug.namewhat) out << "(" << debug.namewhat << ")";
				}
				out << std::endl;
			}
			else
			{
				out << "[ERROR using Lua DEBUG INTERFACE]" << std::endl;
			}
		}

		lua_pushstring(L, out.str().c_str()) ;
		return 1;
	}

	void LuaCallBase::execute(int numArgs, int numOutput, int top)
	{
		SLB_DEBUG_CALL;
		int base = lua_gettop(_L) - numArgs;
		lua_pushcfunction(_L, LuaCallBase::errorHandler);
		lua_insert(_L, base);

		if(lua_pcall(_L, numArgs, numOutput, base)) 
		{
			std::runtime_error exception( lua_tostring(_L, -1) );
			lua_remove(_L, base);
			lua_settop(_L,top); // TODO: Remove this.
			throw exception;
		}
		lua_remove(_L, base);
	}
*/
}
