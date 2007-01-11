#ifndef __SLB_CLASS__
#define __SLB_CLASS__

#include "Export.hpp"
#include "Object.hpp"
#include "ref_ptr.hpp"
#include "FuncCall.hpp"
#include "ClassHelpers.hpp"
#include "Table.hpp"
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

	class Instance;

	class SLB_EXPORT Class : public Namespace {
	public:
		// some typedefs
		typedef void (*Callback) (void*, lua_State*) ;
		typedef void* (*ConvertToBase) (void*);
		typedef std::map< ref_ptr<Class>, ConvertToBase > BaseClassMap;

		const std::type_info *getTypeid() const { return _typeid; }
		const std::string &getName() const      { return _name; }
		void setName(const std::string&);

		void pushInstance(lua_State*, void *obj, bool callGC = true);
		void pushInstance(lua_State*, const void *obj);
		void* getInstance(lua_State*, int pos) const;
		const void* getConstInstance(lua_State*, int pos) const;

		void setPushCallback(Callback c) { _onPushCallback = c; }
		void setGCCallback(Callback c)   { _onGarbageCollection = c; }

		template<class This, class Base>
		void inheritsFrom();

		void setConstructor( FuncCall *constructor );

	protected:
		Class(const std::type_info&);
		virtual ~Class();

		void pushImplementation(lua_State *);
		Instance* getRawInstance(lua_State *, int) const;

		virtual int get(lua_State *L, const std::string &key);
		virtual int __call(lua_State*);
		virtual int __gc(lua_State*);

		const std::type_info *_typeid;
		std::string _name;
		Callback    _onPushCallback;
		Callback    _onGarbageCollection;
		BaseClassMap _baseClasses;
		ref_ptr<FuncCall> _constructor;

	private:
		void pushRawInstance(lua_State *, Instance *);
		bool checkInstance(lua_State *, const void *obj);
		void inheritsFrom(const std::type_info &base, ConvertToBase func);
		void *convertFrom(void *obj, const Class *derived_class) const;

		Class(const Class&);
		Class& operator=(const Class&);

		friend class Manager;
		friend class Instance;
	};

	//--------------------------------------------------------------------
	// Inline implementations:
	//--------------------------------------------------------------------
	
	template<class D, class B>
	struct Conversor
	{
		static void* convert(void *raw_d)
		{
			D* derived = reinterpret_cast<D*>(raw_d);
			B* base = derived;
			return (void*) base;
		}
	};
		
	template<class D, class B>
	inline void Class::inheritsFrom()
	{
		inheritsFrom(typeid(B), &Conversor<D,B>::convert );
	}
}


#endif
