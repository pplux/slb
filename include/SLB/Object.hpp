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

#ifndef __SLB_OBJECT__
#define __SLB_OBJECT__

#include <assert.h>
#include <string>
#include "Export.hpp"

struct lua_State;

namespace SLB
{
	class SLB_EXPORT Object 
	{
	public:
		unsigned int referenceCount() const { return _refCounter; }
		void ref();
		void unref();

		void push(lua_State *L);
		void setInfo(const std::string&);
		const std::string& getInfo() const;

	protected:

		Object();
		virtual ~Object();

		virtual void pushImplementation(lua_State *) = 0;
		virtual void onGarbageCollection(lua_State *) {}

	private:
		void initialize(lua_State *) const;
		static int GC_callback(lua_State *);
		unsigned int _refCounter;
		std::string _info; // for metadata, documentation, ...

		
		Object( const Object &slbo);
		Object& operator=( const Object &slbo);
	};

	// ------------------------------------------------------------
	// ------------------------------------------------------------
	// ------------------------------------------------------------
		
	inline void Object::ref()
	{
		++_refCounter;
	}

	inline void Object::unref()
	{
		assert(_refCounter > 0);
		--_refCounter; 
		if (_refCounter == 0) delete this;
	}

	inline void Object::setInfo(const std::string& s) {_info = s;}
	inline const std::string& Object::getInfo() const {return _info;}

} //end of SLB namespace

#endif
