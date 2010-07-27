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
