#include <SLB/SLB.hpp>
#include "unit_004.hpp"

namespace Unit_004 {

	void wrapper()
	{
		SLB_DEBUG_CALL;
		SLB_DEBUG(1, "--> Loading unit_004 wrapper -->");
		
		SLB::Class<Animal, SLB::Instance::NoCopy >("Unit_004::Animal")
			.set("makeSound", &Animal::makeSound)
		;

		SLB::Class<Duck>("Unit_004::Duck")
			.constructor<bool>()
			.set("canFly", &Duck::canFly)
			.inherits<Animal>()
		;

		SLB::Class<Dog>("Unit_004::Dog")
			.constructor<bool>()
			.set("bites", &Dog::bites)
			.inherits<Animal>()
		;

		
		SLB_DEBUG(1, "<-- Loading unit_004 wrapper <--");
	}


	SLB_ON_LOAD(wrapper)

}
