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

#ifndef __SLB_SCRIPT__
#define __SLB_SCRIPT__

#include "lua.hpp" 
#include "PushGet.hpp"
#include "Type.hpp"
#include<stdexcept>

namespace SLB {
	
	class SLB_EXPORT Script
	{	
	public:
		Script(bool loadDefaultLibs = true);
		virtual ~Script();
		void doFile(const std::string &filename) throw (std::exception);
		void doString(const std::string &codeChunk, const std::string &where_hint ="[SLB]") throw (std::exception);

		 /* ************************* WARNING *********************************
		  * Sometines you need to manually call Garbage Collector(GC), to be sure
		  * that all objects are destroyed. This is mandatory when using smartPointers
		  * be very carefull. GC operations are really expensive, avoid calling GC
		  * too frequently.
		  * ************************* WARNING *********************************/
		void callGC();

		/// Returns the number of Kb of memory used by the script
		size_t memUsage();

		template<class T>
		void set(const std::string &name, T value)
		{ SLB::setGlobal<T>(getState(), value, name.c_str());}

		template<class T>
		T get(const std::string&name)
		{ return SLB::getGlobal<T>(getState(), name.c_str()); }

	protected:
		virtual void onNewState(lua_State *L) {}
		virtual void onCloseState(lua_State *L) {}
		virtual void onGC(lua_State *L) {}

		lua_State* getState();
		void close(); // will close lua_state

	private:
		Script(const Script &s);
		Script& operator=(const Script&);
		lua_State *_L;
		bool _loadDefaultLibs;
	};

}

#endif
