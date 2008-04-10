#include <SLB/SLB.hpp>
#include "unit_003.hpp"

namespace Unit_003 {

	void wrapper()
	{
		SLB_DEBUG_CALL;
		SLB_DEBUG(1, "--> Loading unit_003 wrapper -->");
		
		SLB::Class<MapIntString>("Unit_003::MapIntString")
			.constructor()
			.object__index( &MapIntString::get )
			.object__newindex( &MapIntString::set )
		;
		
		SLB_DEBUG(1, "<-- Loading unit_003 wrapper <--");
	}


	SLB_ON_LOAD(wrapper)

}
