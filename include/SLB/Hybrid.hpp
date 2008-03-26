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

#include "Export.hpp"
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

	class SLB_EXPORT HybridBase {
	public:

		/** LinkFromfile will load a file, create a new lua_state and search global functions there
		 * for hybrid methods ("LCall" calls) */
		bool linkFromFile(const char *file);
		/** LinkfromMemory performs the same as LinkFromFile but without reading a file. */
		bool linkFromMemory(const char *buffer);
		/** Will use a given luaState and a table to search (hybrid-)methods */
		bool linkFromLuaTable(lua_State *L, int pos);

		/** That function will call linkFromLuaTable, but can easily wrapped :) */
		static int lua_linkFromLuaTable(lua_State *L);

		/** returns the luaState used... DO NOT USE in case this object creates its own
		 * state because the luaState will be eventually destroyed.*/
		lua_State *getLuaState() { return _L; }

	protected:
		typedef std::map< const char *, LuaCallBase *> MethodMap;

		HybridBase();
		virtual ~HybridBase();

		/** Will be called each time the instance needs a new state (its own state):
		 *    - here you can open lua default tables (it is not done by default).
		 *    - this function will be called only when needed, for example
		 *    it will NOT be called with linkFromLuaTable. 
		 */
		virtual void onInitState(lua_State *L) {}

		/// Pushes onto the lua_stack the function, if exists returns true.
		bool pushFunction(const char *name);

		lua_State *_L;
		int _table_ref;
		MethodMap  _methods;
	private:

		void clearData();
		void initState();
		bool link(const char *errMSG);
		bool _ownState;
	};

	template<class BaseClass>
	class Hybrid : public virtual HybridBase {
	public:
		Hybrid() : HybridBase()
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
			// Basic hybrid functions:
			c->set("linkFromMemory", FuncCall::create(&HybridBase::linkFromMemory));
			c->set("linkFromFile",   FuncCall::create(&HybridBase::linkFromFile));
			c->set("linkFromTable",  FuncCall::create(HybridBase::lua_linkFromLuaTable));
			c->inheritsFrom<BaseClass,HybridBase>();
		}
		virtual ~Hybrid() {}
	protected:
		
	#define SLB_ARG_H(N) ,T##N arg_##N
	#define SLB_ARG(N) , arg_##N
	#define SLB_REPEAT(N) \
	\
		template<class R SPP_COMMA_IF(N) SPP_ENUM_D(N, class T)> \
		R LCall( const char *name SPP_REPEAT(N, SLB_ARG_H) ) \
		{ \
			typedef SLB::LuaCall<R(BaseClass* SPP_COMMA_IF(N) SPP_ENUM_D(N,T))> LC;\
			LC *method = 0; \
			SLB_DEBUG(3,"Call Hybrid-method [%s]", name)\
			MethodMap::iterator it = _methods.find(name) ; \
			if (it != _methods.end()) \
			{ \
				method = reinterpret_cast<LC*>(it->second); \
				SLB_DEBUG(4,"method [%s] was found %x", name,method)\
			} \
			else \
			{ \
				if (pushFunction(name)) \
				{ \
					method = new LC(_L, -1);\
					SLB_DEBUG(2,"method [%s] found in lua [OK] -> %x", name,method)\
					_methods[name] = method;\
				} \
				else \
				{ \
					_methods[name] = 0L; \
					SLB_DEBUG(2,"method [%s] found in lua [FAIL!]", name)\
				}\
			} \
			if (method) return (*method)(static_cast<BaseClass*>(this) SPP_REPEAT(N, SLB_ARG) ); \
			return R(); \
		} \

	SPP_MAIN_REPEAT_Z(MAX,SLB_REPEAT)
	#undef SLB_REPEAT
	#undef SLB_ARG
	#undef SLB_ARG_H
	};
}

#endif
