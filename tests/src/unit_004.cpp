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
