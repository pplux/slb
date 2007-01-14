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
		typedef void* (*ConvertToBase) (void*);
		typedef std::map< ref_ptr<ClassInfo>, ConvertToBase > BaseClassMap;

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
		void inheritsFrom(const std::type_info &base, ConvertToBase func);
		void *convertFrom(void *obj, const ClassInfo *derived_class) const;
		void pushInstance(lua_State *L, InstanceBase *instance);
		InstanceBase* getInstance(lua_State *L, int pos) const;
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
	inline void ClassInfo::inheritsFrom()
	{
		inheritsFrom(typeid(B), &Conversor<D,B>::convert );
	}

	/*
	class SLB_EXPORT ClassInfo : public Namespace {
	public:
		// some typedefs
		typedef void (*Callback) (void*, lua_State*) ;
		typedef void* (*ConvertToBase) (void*);
		typedef void* (*GetHandler)(void*);
		typedef void* (*DestroyHandler)(void*);
		typedef std::map< ref_ptr<ClassInfo>, ConvertToBase > BaseClassMap;


		void push_ptr(lua_State *L, void *ptr);
		void push_const_ptr(lua_State *L, const void *ptr);
		void push_ref(lua_State *L, void *ptr);
		void push_copy(lua_State *L, const void *ptr);

		// DEPRECATED
		void pushInstance(lua_State*, void *obj, bool callGC = true);
		void pushInstance(lua_State*, const void *obj);

		void setPushCallback(Callback c) { _onPushCallback = c; }
		void setGCCallback(Callback c)   { _onGarbageCollection = c; }



		void setHandler_ptr( GetHandler h)         { _handler_ptr = h; }
		void setHandler_const_ptr( GetHandler h)   { _handler_const_ptr = h; }
		void setHandler_ref ( GetHandler h)        { _handler_ref = h; }
		void setHandler_const_ref( GetHandler h)   { _handler_const_ref = h; }
		void setHandler_copy( GetHandler h)        { _handler_copy = h; }
		void setDestroyHandler( DestroyHandler dh) { _destroy_handler = dh; }

	protected:
		ClassInfo(const std::type_info&);
		virtual ~ClassInfo();

		void pushImplementation(lua_State *);
		Instance* getRawInstance(lua_State *, int) const;

		virtual int get(lua_State *L, const std::string &key);
		virtual int __call(lua_State*);
		virtual int __gc(lua_State*);

		const std::type_info *_typeid;
		std::string _name;
		Callback    _onPushCallback;
		Callback    _onGarbageCollection;

		GetHandler _handler_ptr;
		GetHandler _handler_const_ptr;
		GetHandler _handler_ref;
		GetHandler _handler_const_ref;
		GetHandler _handler_copy;

		DestroyHandler _destroy_handler;

	private:
		void pushRawInstance(lua_State *, Instance *);
		bool checkInstance(lua_State *, const void *obj);
		void inheritsFrom(const std::type_info &base, ConvertToBase func);
		void *convertFrom(void *obj, const ClassInfo *derived_class) const;

		ClassInfo(const ClassInfo&);
		ClassInfo& operator=(const ClassInfo&);

		friend class Manager;
		friend class Instance;
	};


	*/
}


#endif
