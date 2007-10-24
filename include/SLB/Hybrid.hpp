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

#ifndef __SLB_HYBRID__
#define __SLB_HYBRID__

#include "SPP.hpp"
#include "Manager.hpp"
#include "LuaCall.hpp"
#include "Value.hpp"
#include "ClassInfo.hpp"
#include "Instance.hpp"
#include <typeinfo>
#include <map>
#include <vector>
#include <string>

struct lua_State;

namespace SLB {

	class HybridBase {
	public:

		bool linkFromFile(const char *file);
		bool linkFromMemory(const char *buffer);
		lua_State *getLuaState() { return _L; }

	protected:
		HybridBase(lua_State *L);
		virtual ~HybridBase();

		lua_State *_L;
	};

	template<class BaseClass>
	class Hybrid : public HybridBase {
	public:
		Hybrid(lua_State *L = 0L) : HybridBase(L)
		{
			ClassInfo *c;
			c = Manager::getInstance().getOrCreateClass( typeid(BaseClass) );
			if (!c->initialized())
			{
				// Give a default instance factory... that only is able
				// to handle push/get of pointers without handling 
				// construction, copy, delete, ...
				c->setInstanceFactory(
					new InstanceFactoryAdapter< BaseClass,
						Instance::NoCopyNoDestroy::Implementation<BaseClass> >()
				);
			}
		}
	protected:
		
	#define SLB_ARG_H(N) ,T##N arg_##N
	#define SLB_ARG(N) , arg_##N
	#define SLB_REPEAT(N) \
	\
		template<class R SPP_COMMA_IF(N) SPP_ENUM_D(N, class T)> \
		R LCall( const char *name SPP_REPEAT(N, SLB_ARG_H) ) \
		{ \
			SLB::LuaCall<R(BaseClass* SPP_COMMA_IF(N) SPP_ENUM_D(N,T))> obj(_L, name); \
			return obj(static_cast<BaseClass*>(this) SPP_REPEAT(N, SLB_ARG) ); \
		} \

	SPP_MAIN_REPEAT_Z(MAX,SLB_REPEAT)
	#undef SLB_REPEAT
	#undef SLB_ARG
	#undef SLB_ARG_H
	};
}

#endif
