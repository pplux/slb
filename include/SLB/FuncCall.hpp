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



#ifndef __SLB_FUNCCALL__
#define __SLB_FUNCCALL__

#include "Object.hpp"
#include "Export.hpp"
#include "Allocator.hpp"
#include "String.hpp"
#include "SPP.hpp"
#include "lua.hpp"

#include <vector>
#include <typeinfo>

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
		\
			/* FunCall Class constructors  */ \
			template<class C SPP_COMMA_IF(N) SPP_ENUM_D(N, class T)> \
			static FuncCall* classConstructor(); \

		SPP_MAIN_REPEAT_Z(MAX,SLB_REPEAT)
		#undef SLB_REPEAT

		/* special case of a proper lua Function */
		static FuncCall* create(lua_CFunction f);

		size_t getNumArguments() const { return _Targs.size(); }
		const std::type_info* getArgType(size_t p) const { return _Targs[p].first; }
		const String& getArgComment(size_t p) const { return _Targs[p].second; }
		const std::type_info* getReturnedType() const { return _Treturn; }
		void setArgComment(size_t p, const String& c);

	protected:
		FuncCall();
		virtual ~FuncCall();
	
		void pushImplementation(lua_State *L);
		virtual int call(lua_State *L) = 0;

		typedef std::pair<const std::type_info*, SLB::String> TypeInfoStringPair;
		std::vector< TypeInfoStringPair, Allocator<TypeInfoStringPair> > _Targs;
		const std::type_info* _Treturn;
	private:
		static int _call(lua_State *L);

	friend class Manager;	
	friend class ClassInfo;	
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
		typedef Private::FC_ConstMethod<C,R(SPP_ENUM_D(N,T))> _type_;\
		return new (Malloc(sizeof(_type_))) _type_(func); \
	} \
	template<class C,class R SPP_COMMA_IF(N) SPP_ENUM_D(N, class T)> \
	inline FuncCall* FuncCall::createNonConst(R (C::*func)(SPP_ENUM_D(N,T)) ) \
	{ \
		typedef Private::FC_Method<C,R(SPP_ENUM_D(N,T))> _type_;\
		return new (Malloc(sizeof(_type_))) _type_(func); \
	} \
	\
	template<class R SPP_COMMA_IF(N) SPP_ENUM_D(N, class T)> \
	inline FuncCall* FuncCall::create(R (*func)(SPP_ENUM_D(N,T)) ) \
	{ \
		typedef Private::FC_Function<R(SPP_ENUM_D(N,T))> _type_;\
		return new (Malloc(sizeof(_type_))) _type_(func); \
	} \
	\
	template<class C SPP_COMMA_IF(N) SPP_ENUM_D(N, class T)> \
	inline FuncCall* FuncCall::classConstructor() \
	{ \
		typedef Private::FC_ClassConstructor<C(SPP_ENUM_D(N,T))> _type_;\
		return new (Malloc(sizeof(_type_))) _type_; \
	} \
	
	SPP_MAIN_REPEAT_Z(MAX,SLB_REPEAT)
	#undef SLB_REPEAT

}
#endif
