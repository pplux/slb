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

#ifndef __SLB_VALUE__
#define __SLB_VALUE__

#include "Object.hpp"
#include "PushGet.hpp"

struct lua_State;

namespace SLB {

	template<class T> class RefValue;
	template<class T> class AutoDeleteValue;
	template<class T> class CopyValue;

	namespace Value
	{
		// holds a reference to an object
		template<class T> inline Object* ref( T& obj )
		{ return new RefValue<T>(obj); }

		// holds a reference to a const object
		template<class T> inline Object* ref( const T& obj )
		{ return new RefValue<T>(obj); }
		
		// holds a reference to an object (ptr)
		template<class T> inline Object* ref( T *obj )
		{ return new RefValue<T>(obj); }
		
		// holds a reference to a const object (const ptr)
		template<class T> inline Object* ref( const T *obj )
		{ return new RefValue<T>(obj); }

		// creates a copy of the given object
		template<class T> inline Object* copy( const T &obj)
		{ return new CopyValue<T>(obj); }

		// holds a ptr to an object, the object will be automatically
		// deleted.
		template<class T> inline Object* autoDelete( T *obj )
		{ return new AutoDeleteValue<T>(obj); }
	}

	template<class T>
	class RefValue: public virtual Object {
	public:
		RefValue( T& obj );
		RefValue( const T& obj );
		RefValue( T *obj );
		RefValue( const T *obj );


	protected:
		void pushImplementation(lua_State *L);
		virtual ~RefValue() {}
	private:
		union {
			T* _obj;
			const T* _constObj;
		};
		bool _isConst;
	};

	template<class T>
	class CopyValue : public virtual Object {
	public:
		CopyValue( const T& obj );
	protected:
		void pushImplementation(lua_State *L);
		virtual ~CopyValue() {}
	private:
		T _obj;
	};

	template<class T>
	class AutoDeleteValue : public virtual Object {
	public:
		AutoDeleteValue( T *obj );
	protected:
		void pushImplementation(lua_State *L);
		virtual ~AutoDeleteValue();
	private:
		T *_obj;

	};

	//--------------------------------------------------------------------
	// Inline implementations:
	//--------------------------------------------------------------------
	
	template<class T>
	inline RefValue<T>::RefValue( T& obj ) : 
		_obj(&obj), _isConst(false)
	{
	}

	template<class T>
	inline RefValue<T>::RefValue( const T& obj ) : 
		_constObj(&obj), _isConst(true)
	{
	}

	template<class T>
	inline RefValue<T>::RefValue( T *obj ) : 
		_obj(obj), _isConst(false)
	{
	}

	template<class T>
	inline RefValue<T>::RefValue( const T *obj ) : 
		_constObj(obj), _isConst(true)
	{
	}
	
	template<class T>
	inline void RefValue<T>::pushImplementation(lua_State *L)
	{
		if (_isConst) SLB::push(L, _constObj);	
		else SLB::push(L, _obj);	
	}

	template<class T>
	inline CopyValue<T>::CopyValue( const T& obj ) : _obj(obj)
	{
	}

	template<class T>
	inline void CopyValue<T>::pushImplementation(lua_State *L)
	{
		SLB::push(L,_obj);
	}

	template<class T>
	inline AutoDeleteValue<T>::AutoDeleteValue( T *obj ) : _obj(obj)
	{
	}
	
	template<class T>
	inline void AutoDeleteValue<T>::pushImplementation(lua_State *L)
	{
		// do not call GC on this object.
		SLB::Private::Type<T*>::push(L, _obj, false);
	}

	template<class T>
	inline AutoDeleteValue<T>::~AutoDeleteValue()
	{
		delete _obj;
	}
}


#endif
