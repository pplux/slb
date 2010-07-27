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


#include "unit_SLB.hpp"

namespace Unit_SLB {

	Table::Table()
	{
		SLB_DEBUG_CALL;
		SLB_DEBUG(2, "Constructor Table(%p)", this);
	}

	Table::~Table()
	{
		SLB_DEBUG_CALL;
		SLB_DEBUG(2, "Destructor Table(%p)", this);
	}

	int Table::class__index(lua_State *L)
	{
		SLB_DEBUG_CALL;
		SLB_DEBUG(2, "Test __index metamethod & getCache");
		SLB::ClassInfo *c = SLB::Manager::getInstance().getClass(L,1);
		if (c)
		{
			c->getCache(L);
		}
		else
		{
			luaL_error(L, "Invalid class at #1");
		}
		return 1;
	}

	int Table::class__newindex(lua_State *L)
	{
		SLB_DEBUG_CALL;
		SLB_DEBUG(2, "Test __newindex metamethod & setCache");
		SLB::ClassInfo *c = SLB::Manager::getInstance().getClass(L,1);
		if (c)
		{
			c->setCache(L);
		}
		else
		{
			luaL_error(L, "Invalid class at #1");
		}
		return 0;
	}

	bool Checks::checkHybridBase()
	{
		SLB::ClassInfo *ci = SLB::Manager::getInstance().getClass(typeid(SLB::HybridBase));
		return (ci != 0L);
	}

	void wrapper()
	{
		SLB_DEBUG_CALL;
		SLB_DEBUG(1, "--> Loading unit_SLB wrapper -->");
		

		SLB::Class< Table, SLB::Instance::NoCopy >("Unit_SLB::Table")
			.class__index( &Table::class__index )
			.class__newindex( &Table::class__newindex )
			.constructor()
		;

		SLB::Class<Checks>("Unit_SLB::Checks")
			.set("checkHybridBase", &Checks::checkHybridBase)
			.constructor()
		;

		SLB_DEBUG(1, "<-- Loading unit_SLB wrapper <--");
	}


	SLB_ON_LOAD(wrapper)

}
