#ifndef __SLB_FUNCCALL__
#define __SLB_FUNCCALL__

#include "Object.hpp"
#include "Export.hpp"
#include "SPP.hpp"
#include "lua.hpp"

#include <vector>

namespace SLB
{
	class SLB_EXPORT FuncCall : public Object
	{
	public:

		#define SLB_REPEAT(N) \
		\
			/* FunCall for class Methods */ \
			template<class C,class R SPP_COMMA_IF(N) SPP_ENUM_D(N, class T)> \
			static FuncCall* create(R (C::*func)(SPP_ENUM_D(N,T)) ); \
		\
			/* FunCall for CONST class Methods */ \
			template<class C,class R SPP_COMMA_IF(N) SPP_ENUM_D(N, class T)> \
			static FuncCall* create(R (C::*func)(SPP_ENUM_D(N,T)) const ); \
		\
			/* (explicit) FunCall for CONST class Methods */ \
			template<class C,class R SPP_COMMA_IF(N) SPP_ENUM_D(N, class T)> \
			static FuncCall* createConst(R (C::*func)(SPP_ENUM_D(N,T)) const ); \
		\
			/* (explicit) FunCall for NON-CONST class Methods */ \
			template<class C,class R SPP_COMMA_IF(N) SPP_ENUM_D(N, class T)> \
			static FuncCall* createNonConst(R (C::*func)(SPP_ENUM_D(N,T))); \
		\
			/* FunCall for C-functions  */ \
			template<class R SPP_COMMA_IF(N) SPP_ENUM_D(N, class T)> \
			static FuncCall* create(R (func)(SPP_ENUM_D(N,T)) ); \

		SPP_MAIN_REPEAT_Z(MAX,SLB_REPEAT)
		#undef SLB_REPEAT

		size_t getNumArguments() const { return _Targs.size(); }
		const std::type_info* getArgType(size_t p) const { return _Targs[p]; }
		const std::type_info* getReturnedType() const { return _Treturn; }

		// items to remove from the begining 
		void setRemoveItems(int num) { _remove = num; }
	protected:
		FuncCall(int items_to_remove = 0);
		virtual ~FuncCall();
	
		void pushImplementation(lua_State *L);
		virtual int call(lua_State *L) = 0;

		std::vector< const std::type_info* > _Targs;
		const std::type_info* _Treturn;
	private:
		static int _call(lua_State *L);
		int _remove;

	friend class Manager;	
	friend class Class;	
	};

} //end of SLB namespace

	//--------------------------------------------------------------------
	// Inline implementations:
	//--------------------------------------------------------------------
	
	#include "Private_FuncCall.hpp"
	
	#define SLB_REPEAT(N) \
	\
	template<class C,class R SPP_COMMA_IF(N) SPP_ENUM_D(N, class T)> \
	inline FuncCall* FuncCall::create(R (C::*func)(SPP_ENUM_D(N,T)) ) \
	{ \
		return createNonConst(func); \
	} \
	\
	template<class C,class R SPP_COMMA_IF(N) SPP_ENUM_D(N, class T)> \
	inline FuncCall* FuncCall::create(R (C::*func)(SPP_ENUM_D(N,T)) const ) \
	{ \
		return createConst(func); \
	} \
	\
	template<class C,class R SPP_COMMA_IF(N) SPP_ENUM_D(N, class T)> \
	inline FuncCall* FuncCall::createConst(R (C::*func)(SPP_ENUM_D(N,T)) const ) \
	{ \
		return new Private::FC_ConstMethod<C,R(SPP_ENUM_D(N,T))>(func); \
	} \
	template<class C,class R SPP_COMMA_IF(N) SPP_ENUM_D(N, class T)> \
	inline FuncCall* FuncCall::createNonConst(R (C::*func)(SPP_ENUM_D(N,T)) ) \
	{ \
		return new Private::FC_Method<C,R(SPP_ENUM_D(N,T))>(func); \
	} \
	\
	template<class R SPP_COMMA_IF(N) SPP_ENUM_D(N, class T)> \
	inline FuncCall* FuncCall::create(R (*func)(SPP_ENUM_D(N,T)) ) \
	{ \
		return new Private::FC_Function<R(SPP_ENUM_D(N,T))>(func);\
	} \
	
	SPP_MAIN_REPEAT_Z(MAX,SLB_REPEAT)
	#undef SLB_REPEAT

}
#endif
