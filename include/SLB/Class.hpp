#ifndef __SLB_CLASS__
#define __SLB_CLASS__

#include "SPP.hpp"
#include "Export.hpp"
#include "ClassInfo.hpp"
#include "ClassHelpers.hpp"
#include "Manager.hpp"
#include "FuncCall.hpp"
#include "Value.hpp"
#include <typeinfo>
#include <map>
#include <vector>
#include <string>

struct lua_State;

namespace SLB {
	
	template<class T>
	struct DefaultWrapper
	{
		static void onPush(void *, lua_State *) {}
		static void onGC(void *raw_obj, lua_State*)    { T *obj = reinterpret_cast<T*>(raw_obj); delete obj; }
	};

	template< typename T, typename W = DefaultWrapper<T> >
	class Class {
	public:
		Class(const char *name);

		Class<T,W> &rawSet(const char *name, Object *obj);

		template<typename TValue>
		Class<T,W> &set(const char *name, const TValue &obj)
		{ return rawSet(name, (Object*) Value::copy(obj)); }

		template<typename TValue>
		Class<T,W> &set_ref(const char *name, TValue obj)
		{ return rawSet(name, Value::ref(obj)); }

		template<typename TValue>
		Class<T,W> &set_autoDelete(const char *name, TValue *obj)
		{ return rawSet(name, Value::autoDelete(obj)); }

		Class<T,W> &constructor();

		template<typename TBase>
		Class<T,W> &inherits()
		{ _class->inheritsFrom<T,TBase>(); return *this;}

		#define SLB_REPEAT(N) \
		\
			/* Methods */ \
			template<class R SPP_COMMA_IF(N) SPP_ENUM_D(N, class T)> \
			Class<T,W> &set(const char *name, R (T::*func)(SPP_ENUM_D(N,T)) ); \
		\
			/* CONST Methods */ \
			template<class R SPP_COMMA_IF(N) SPP_ENUM_D(N, class T)> \
			Class<T,W> &set(const char *name, R (T::*func)(SPP_ENUM_D(N,T)) const ); \
		\
			/* C-functions  */ \
			template<class R SPP_COMMA_IF(N) SPP_ENUM_D(N, class T)> \
			Class<T,W> &set(const char *name, R (func)(SPP_ENUM_D(N,T)) ); \
		\
			/* constructors */ \
			template<class T0 SPP_COMMA_IF(N) SPP_ENUM_D(N, class T)> \
			Class<T,W> &constructor(); \

		SPP_MAIN_REPEAT_Z(MAX,SLB_REPEAT)
		#undef SLB_REPEAT

	protected:
		ClassInfo *_class;
	
	};
	
	template<typename T, typename W>
	Class<T,W>::Class(const char *name)
	{
		_class = Manager::getInstance().getOrCreateClass( typeid(T) );
		_class->setName( name );
		_class->setGCCallback( W::onGC );
		_class->setPushCallback( W::onPush );
	}
	
	template<typename T, typename W>
	inline Class<T,W> &Class<T,W>::rawSet(const char *name, Object *obj)
	{
		_class->set(name, obj);
		return *this;
	}
	
	template<typename T, typename W>
	inline Class<T,W> &Class<T,W>::constructor()
	{
		_class->setConstructor( Constructor<T()>::create() );
		return *this;
	}

	#define SLB_REPEAT(N) \
	\
		/* Methods */ \
		template<typename T, typename W> \
		template<class R SPP_COMMA_IF(N) SPP_ENUM_D(N, class T)> \
		inline Class<T,W> &Class<T,W>::set(const char *name, R (T::*func)(SPP_ENUM_D(N,T)) ){ \
			return rawSet(name, FuncCall::create(func)); \
		} \
	\
		/* CONST Methods */ \
		template<typename T, typename W> \
		template<class R SPP_COMMA_IF(N) SPP_ENUM_D(N, class T)> \
		inline Class<T,W> &Class<T,W>::set(const char *name, R (T::*func)(SPP_ENUM_D(N,T)) const ){ \
			return rawSet(name, FuncCall::create(func)); \
		} \
	\
		/* C-functions  */ \
		template<typename T, typename W> \
		template<class R SPP_COMMA_IF(N) SPP_ENUM_D(N, class T)> \
		inline Class<T,W> &Class<T,W>::set(const char *name, R (func)(SPP_ENUM_D(N,T)) ){ \
			return rawSet(name, FuncCall::create(func)); \
		} \
	\
		/* constructor  */ \
		template<typename T, typename W> \
		template<class T0 SPP_COMMA_IF(N) SPP_ENUM_D(N, class T)> \
		inline Class<T,W> &Class<T,W>::constructor(){ \
			_class->setConstructor( Constructor<T(T0 SPP_COMMA_IF(N) SPP_ENUM_D(N,T))>::create());\
			return *this; \
		} \

	SPP_MAIN_REPEAT_Z(MAX,SLB_REPEAT)
	#undef SLB_REPEAT
}


#endif
