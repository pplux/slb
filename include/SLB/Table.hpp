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

#ifndef __SLB_TABLE__
#define __SLB_TABLE__

#include "Export.hpp"
#include "Object.hpp"
#include "ref_ptr.hpp"
#include <typeinfo>
#include <map>
#include <string>

struct lua_State;

namespace SLB {

	class SLB_EXPORT Table : public virtual Object {
	public:
		typedef std::map< std::string, ref_ptr<Object> > Elements;
		Table(const std::string &separator = "", bool cacheable = false);

		void erase(const std::string &);
		Object* get(const std::string &);
		void set(const std::string &, Object*);

		bool isCacheable() { return _cacheable; }

	protected:
		virtual ~Table();

		Object* rawGet(const std::string &);
		void rawSet(const std::string &, Object*);
		
		void pushImplementation(lua_State *);

		virtual int get(lua_State *L, const std::string &key);
		virtual int get(lua_State *L, long int number);

		virtual int __index(lua_State *L);
		virtual int __newindex(lua_State *L);
		virtual int __call(lua_State *L);
		virtual int __garbageCollector(lua_State *L);
		virtual int __tostring(lua_State *L);

		Elements _elements;

	private:
		typedef std::pair<Table*,const std::string> TableFind;
		typedef int (Table::*TableMember)(lua_State*);
		static int __meta(lua_State*);
		void pushMeta(lua_State *L, TableMember) const;

		TableFind getTable(const std::string &key, bool create);

		bool _cacheable;
		std::string _separator;

		Table(const Table&);
		Table& operator=(const Table&);
	};

	//--------------------------------------------------------------------
	// Inline implementations:
	//--------------------------------------------------------------------
		
	inline void Table::erase(const std::string &name) { set(name, 0); }
}


#endif
