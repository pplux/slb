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

#ifndef __SLB_TYPEINFO_WRAPPER__
#define __SLB_TYPEINFO_WRAPPER__

#include <typeinfo>

namespace SLB {

	class TypeInfoWrapper
	{
	public:

		TypeInfoWrapper() :
			_ID(0), _type( &typeid(TypeInfoWrapper) )
		{
		}

		TypeInfoWrapper(const std::type_info &t) :
			_ID(0), _type(&t)
		{
			for (const char *__s = name() ; *__s; ++__s)
				_ID = 5 * _ID + *__s;
		}

		const std::type_info& type() const { return *_type; }

		const char *name() const { return _type->name(); }

		bool operator<(const TypeInfoWrapper &o) const
		{
		#if 0
			// very probable:
			if (_ID != o._ID) return _ID < o._ID;
			if (*_type == *o._type) return false;
			// Highly unprobable:
			return strcmp(_type->name(), o._type->name()) < 0;
		#else
			return _type < o._type;
		#endif
		}
		
	private:
		unsigned long _ID;
		const std::type_info *_type;
	};

#define _TIW(x) TypeInfoWrapper(x)

} // end of SLB Namespace

#endif
