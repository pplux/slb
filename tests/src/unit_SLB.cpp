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


	void wrapper()
	{
		SLB_DEBUG_CALL;
		SLB_DEBUG(1, "--> Loading unit_SLB wrapper -->");
		

		SLB::Class< Table, SLB::Instance::NoCopy >("Unit_SLB::Table")
			.class__index( &Table::class__index )
			.class__newindex( &Table::class__newindex )
			.constructor()
		;

		SLB_DEBUG(1, "<-- Loading unit_SLB wrapper <--");
	}


	SLB_ON_LOAD(wrapper)

}
