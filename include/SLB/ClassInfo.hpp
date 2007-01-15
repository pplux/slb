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
		ClassInfo(const std::type_info&);

		const std::type_info *getTypeid() const { return _typeid; }
		const std::string &getName() const      { return _name; }
		void setName(const std::string&);

		void push_ref(lua_State *L, void *ref);
		void push_ptr(lua_State *L, void *ptr, bool fromConstructor = false);
		void push_const_ptr(lua_State *L, const void *const_ptr);
		void push_copy(lua_State *L, const void *ptr);
		void* get_ptr(lua_State*, int pos);
		const void* get_const_ptr(lua_State*, int pos);

		template<class This, class Base>
		void inheritsFrom();

		void setConstructor( FuncCall *constructor );
		void setInstanceFactory( InstanceFactory *);

	protected:
		virtual ~ClassInfo();
		void pushImplementation(lua_State *);
		virtual int __call(lua_State*);
		virtual int __gc(lua_State*);

		const std::type_info *_typeid;
		std::string       _name;
		InstanceFactory  *_instanceFactory;
		BaseClassMap      _baseClasses;
		ref_ptr<FuncCall> _constructor;

	private:
		void pushInstance(lua_State *L, InstanceBase *instance);
		InstanceBase* getInstance(lua_State *L, int pos) const;
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
