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

namespace SLB {
	
	class SLB_EXPORT Script
	{	
	public:
		Script(bool loadDefaultLibs = true);
		~Script();
		void doFile(const std::string &filename);
		void doString(const std::string &codeChunk, const std::string &where_hint ="[SLB]");

		template<class T>
		void set(const std::string &name, T value) { SLB::setGlobal<T>(L, value, name.c_str()); }

		template<class T>
		T get(const std::string&name) { return SLB::getGlobal<T>(L, name); }

	protected:
		lua_State* getState() { return L; }

	private:
		lua_State *L;
	};

}

#endif
