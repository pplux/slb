/*
    SLB - Simple Lua Binder
    Copyright (C) 2007-2010 Jose L. Hidalgo Valiño (PpluX)

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
	
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
		#ifndef __GNUC__
			// a hash function based on type's name
			for (const char *__s = name() ; *__s; ++__s)
				_ID = 5 * _ID + *__s;
		#else
			_ID = (unsigned long) _type;
		#endif
		}

		const std::type_info& type() const { return *_type; }

		const char *name() const { return _type->name(); }

		bool operator<(const TypeInfoWrapper &o) const
		{
		#ifndef __GNUC__
			// very probable:
			if (_ID != o._ID) return _ID < o._ID;
			if (*_type == *o._type) return false;
			// Highly unprobable:
			return strcmp(_type->name(), o._type->name()) < 0;
		#else
			return _type < o._type;
		#endif
		}

		bool operator==(const TypeInfoWrapper &o) const
		{
			return *_type == *o._type;
		}
		
	private:
		unsigned long _ID;
		const std::type_info *_type;
	};

#define _TIW(x) TypeInfoWrapper(typeid(x))

} // end of SLB Namespace

#endif
