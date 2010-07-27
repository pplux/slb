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


#include "unit_006.hpp"

namespace Unit_006 {

	Test::Test() : _value(Test::A)
	{
	}

	void Test::set(Test::Enum v) { _value = v; }
	Test::Enum Test::get() const { return _value; }
	bool Test::equal(Test::Enum v) const { return v == _value; }
	bool Test::isA() const { return _value == A; }
	bool Test::isB() const { return _value == B; }
	bool Test::isC() const { return _value == C; }



	void wrapper()
	{
		SLB_DEBUG_CALL;
		SLB_DEBUG(1, "--> Loading unit_006 wrapper -->");
		
		SLB::Class< Test >("Unit_006::Test")
			.constructor()
			.set("set", &Test::set)
			.set("get", &Test::get)
			.set("equal", &Test::equal)
			.set("isA", &Test::isA)
			.set("isB", &Test::isB)
			.set("isC", &Test::isC)
			.enumValue("Enum::A", Test::A)
			.enumValue("Enum::B", Test::B)
			.enumValue("Enum::C", Test::C)
		;

		SLB_DEBUG(1, "<-- Loading unit_006 wrapper <--");
	}


	SLB_ON_LOAD(wrapper)

}
