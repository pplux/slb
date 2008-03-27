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

		/** That function will call linkFromLuaTable, it can easily be wrapped :) */
		static int lua_linkFromLuaTable(lua_State *L);

	protected:
		typedef std::map< const char *, LuaCallBase *> MethodMap;

		HybridBase();
		virtual ~HybridBase();

		/** This method will be called each time this instance creates a new
		 * lua_State. Here you can add initialization code, load default libraries,
		 * etc. */
		virtual void onNewState(lua_State *) {}

		/** This method allows to lock the lua_State when the state is not owned
		 * by the instance (when the link was done using linkFromLuaTable). Here
		 * you can add lock mechanism to avoid two instances accessing the same
		 * lua_State */
		virtual void lockBegin(lua_State *) {}

		/** This method is called at the end of the call, see lockBegin */
		virtual void lockEnd(lua_State *) {}

		//---------------------------------------------------------------------
		///--- For Internal use ONLY -------------------------------------------
		//---------------------------------------------------------------------
		/// Pushes onto the lua_stack the function, if exists returns true.
		bool pushFunction(const char *name);

		// This class helps to handle the lockBegin, lockEnd. Using methods
		// directly will require to split LCall to handle return of void or not.
		// (this is a little trick)
		struct AutoLock
		{
			AutoLock(HybridBase *h) : _hybrid(h)
			{
				if (!_hybrid->_ownState)
				{
					SLB_DEBUG(6, "Lock state %p to access hybrid method (%p)", _hybrid->_L, (void*) _hybrid);
					_hybrid->lockBegin( _hybrid->_L );
				}
			}
			~AutoLock()
			{
				if (!_hybrid->_ownState)
				{
					SLB_DEBUG(6, "Unlock state %p to access hybrid method (%p)", _hybrid->_L, (void*) _hybrid);
					_hybrid->lockEnd( _hybrid->_L);
				}
			}
			HybridBase* _hybrid;
		};

		lua_State *_L;
		MethodMap  _methods;
		//---------------------------------------------------------------------
	private:

		void clearData();
		void initState();
		bool link(const char *errMSG);
		int _table_ref;
		bool _ownState;
	};

	template<class BaseClass>
	class Hybrid : public virtual HybridBase {
	public:
		Hybrid()
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
		virtual ~Hybrid() {}
	private:
	protected:
		
	#define SLB_ARG_H(N) ,T##N arg_##N
	#define SLB_ARG(N) , arg_##N
	#define SLB_REPEAT(N) \
	\
		template<class R SPP_COMMA_IF(N) SPP_ENUM_D(N, class T)> \
		R LCall( const char *name SPP_REPEAT(N, SLB_ARG_H) ) \
		{ \
			typedef SLB::LuaCall<R(BaseClass* SPP_COMMA_IF(N) SPP_ENUM_D(N,T))> LC;\
			AutoLock lock(this); \
			LC *method = 0; \
			SLB_DEBUG(3,"Call Hybrid-method [%s]", name)\
			MethodMap::iterator it = _methods.find(name) ; \
			if (it != _methods.end()) \
			{ \
				method = reinterpret_cast<LC*>(it->second); \
				SLB_DEBUG(4,"method [%s] was found %p", name,method)\
			} \
			else \
			{ \
				if (pushFunction(name)) \
				{ \
					method = new LC(_L, -1);\
					SLB_DEBUG(2,"method [%s] found in lua [OK] -> %p", name,method)\
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
