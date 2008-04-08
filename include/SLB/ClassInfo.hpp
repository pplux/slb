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

#ifndef __SLB_CLASS_INFO__
#define __SLB_CLASS_INFO__

#include "Export.hpp"
#include "Object.hpp"
#include "Instance.hpp"
#include "Table.hpp"
#include "ref_ptr.hpp"
#include "FuncCall.hpp"
//#include "ClassHelpers.hpp"
#include <typeinfo>
#include <map>
#include <vector>
#include <string>

struct lua_State;

namespace SLB {

	class SLB_EXPORT Namespace : public Table
	{
	public:
		Namespace( bool cacheable = true ) : Table("::", cacheable) {}
	protected:
		virtual ~Namespace() {}
	};

	class SLB_EXPORT ClassInfo : public Namespace
	{
	public:
		typedef std::map<const std::type_info*, ref_ptr<ClassInfo> > BaseClassMap;

		const std::type_info *getTypeid() const { return _typeid; }
		const std::string &getName() const      { return _name; }
		void setName(const std::string&);

		void push_ref(lua_State *L, void *ref);
		void push_ptr(lua_State *L, void *ptr, bool fromConstructor = false);
		void push_const_ptr(lua_State *L, const void *const_ptr);
		void push_copy(lua_State *L, const void *ptr);
		void* get_ptr(lua_State*, int pos) const;
		const void* get_const_ptr(lua_State*, int pos) const;

		template<class This, class Base>
		void inheritsFrom();

		void setConstructor( FuncCall *constructor );
		void setInstanceFactory( InstanceFactory *);

		/** __index method will receive:
		 *  - object
		 *  - key */
		void setObject__index( FuncCall* );

		/** __index method will receive:
		 *  - object
		 *  - key
		 *  - value */
		void setObject__newindex( FuncCall* );

		/** Here you can use setCache/getCache methods to
		 * speed up indexing.
		 *
		 * __index method will receive:
		 *  - [table] -> ClassInfo
		 *  - key */
		void setClass__index( FuncCall* );

		/** Here you can use setCache/getCache methods to
		 * speed up indexing.
		 * __index method will receive:
		 *  - [table] -> ClassInfo
		 *  - key
		 *  - value */
		void setClass__newindex( FuncCall* );

		//This is used by some default initializations...
		bool initialized() const { return _instanceFactory != 0; }

		bool isSubClassOf( const ClassInfo* );
		bool hasConstructor() const { return _constructor.valid(); }

		//--Private methods -(not meant to be used)-------------------
		void setHybrid() { _isHybrid = true; }
		FuncCall* getConstructor() { return _constructor.get(); }

	protected:
		ClassInfo(const std::type_info&);
		virtual ~ClassInfo();
		void pushImplementation(lua_State *);
		virtual int __index(lua_State*);
		virtual int __newindex(lua_State*);
		virtual int __call(lua_State*);
		virtual int __garbageCollector(lua_State*);
		virtual int __tostring(lua_State*);

		const std::type_info *_typeid;
		std::string       _name;
		InstanceFactory  *_instanceFactory;
		BaseClassMap      _baseClasses;
		ref_ptr<FuncCall> _constructor;
		ref_ptr<FuncCall> _meta__index[2]; // 0 = class, 1 = object
		ref_ptr<FuncCall> _meta__newindex[2]; // 0 = class, 1 = object
		bool _isHybrid;

	private:
		void pushInstance(lua_State *L, InstanceBase *instance);
		InstanceBase* getInstance(lua_State *L, int pos) const;

		friend class Manager;
	};


	//--------------------------------------------------------------------
	// Inline implementations:
	//--------------------------------------------------------------------
	
		
	template<class D, class B>
	inline void ClassInfo::inheritsFrom()
	{
		Manager::getInstance().template addConversor<D,B>();
		_baseClasses[ &typeid(B) ] = Manager::getInstance().getOrCreateClass(typeid(B));
	}

}


#endif
