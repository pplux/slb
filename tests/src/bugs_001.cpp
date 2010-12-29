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

#include "bugs_001.hpp"
#include <SLB/SLB.hpp>

namespace Bugs_001 {

	namespace Bug
	{
		void Bug::method_a() {SLB_DEBUG_CALL;SLB_DEBUG(1,"Method_A"); std::cout << "Method A" << std::endl; }
		void Bug::method_b() {SLB_DEBUG_CALL;SLB_DEBUG(1,"Method_B"); std::cout << "Method B" << std::endl; }
	};


	void test()
	{
		SLB_DEBUG_CALL;
		SLB_DEBUG(1, "--> Testing bug_001  -->");
		SLB::Manager m;
		
		SLB::Class< Bug::Bug >("Bug::Bug", &m )
			.set("method_a", &Bug::Bug::method_a)
			.set("method_b", &Bug::Bug::method_b)
		;

		SLB::Script s1(&m);
		s1.doString("SLB.using(SLB); Bug.Bug.method_a(); Bug.Bug.method_b();");
		// test without using
		SLB::Script s2(&m);
		s2.doString("SLB.Bug.Bug.method_a(); SLB.Bug.Bug.method_b();");
	}

	void wrapper()
	{
		SLB::Manager::defaultManager()->set( "bugs_001::test", SLB::FuncCall::create(test));
	}

	SLB_ON_LOAD(wrapper)

}
