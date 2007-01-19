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
		void pushMeta(lua_State *L, TableMember);

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
