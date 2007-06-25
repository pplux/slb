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

#ifndef __SLB_MANAGER__
#define __SLB_MANAGER__

#include "Object.hpp"
#include "ref_ptr.hpp"
#include "Export.hpp"
#include <typeinfo>
#include <map>

namespace SLB {

	class ClassInfo;
	class Namespace;

	class SLB_EXPORT Manager
	{
	public:
		typedef std::map< const std::type_info*, ref_ptr<ClassInfo> > ClassMap;
		typedef std::map< const std::string, const std::type_info *> NameMap;
		typedef void* (*Conversor)(void *);
		typedef std::map< std::pair<const std::type_info*, const std::type_info*>, Conversor > ConversionsMap;

		static Manager &getInstance();
		static Manager *getInstancePtr();

		const ClassInfo *getClass(const std::type_info&) const;
		const ClassInfo *getClass(const std::string&) const;
		ClassInfo *getClass(const std::type_info&);
		ClassInfo *getClass(const std::string&);
		ClassInfo *getOrCreateClass(const std::type_info &);

		// set a global value ( will be registered authomatically on every lua_State )
		void set(const std::string &, Object *obj);

		void registerSLB(lua_State *L);

		// convert from C1 -> C2
		void* convert( const std::type_info *C1, const std::type_info *C2, void *obj);
		const void* convert( const std::type_info *C1, const std::type_info *C2, const void *obj);

		Namespace* getGlobals() { return _global.get(); }

	protected:

		void rename(ClassInfo *c, const std::string &new_name);
		void addClass( ClassInfo *c );
		template<class Derived, class Base>
		void addConversor();

	private:
		Manager();
		~Manager();
		Manager(const Manager&);
		Manager& operator=(const Manager&);

		ClassMap _classes;
		NameMap  _names;
		ref_ptr<Namespace> _global;
		ConversionsMap _conversions;

		friend class ClassInfo;
	};
	
	//--------------------------------------------------------------------
	// Inline implementations:
	//--------------------------------------------------------------------
	
	template<class D, class B>
	struct ClassConversor
	{
		static void* convertToBase(void *raw_d)
		{
			D* derived = reinterpret_cast<D*>(raw_d);
			B* base = derived;
			return (void*) base;
		}
		static void* convertToDerived(void *raw_b)
		{
			B* base = reinterpret_cast<B*>(raw_b);
			D* derived = dynamic_cast<D*>(base);
			return (void*) derived;
		}
	};
	
	inline Manager &Manager::getInstance()
	{
		return *getInstancePtr();
	}

	template<class D, class B>
	inline void Manager::addConversor()
	{
		_conversions[ ConversionsMap::key_type(&typeid(D), &typeid(B)) ] = &ClassConversor<D,B>::convertToBase;
		_conversions[ ConversionsMap::key_type(&typeid(B), &typeid(D)) ] = &ClassConversor<D,B>::convertToDerived;
	}

	inline void* Manager::convert( const std::type_info *C1, const std::type_info *C2, void *obj)
	{
		if (C1 == C2) return obj; 

		ConversionsMap::iterator i = _conversions.find( ConversionsMap::key_type(C1,C2) );
		if (i != _conversions.end())
		{
			return i->second( obj );
		}
		return 0;
	}

	inline const void* Manager::convert( const std::type_info *C1, const std::type_info *C2, const void *obj)
	{
		return const_cast<void*>(convert(C1,C2, const_cast<void*>(obj)));
	}
}

#endif
