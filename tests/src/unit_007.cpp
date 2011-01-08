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


#include "unit_007.hpp"
#include <sstream>

namespace Unit_007 {

	int JamesBond::_Total = 0;
	int JamesBond::_Max   = 0;

	JamesBond::JamesBond()
	{
		SLB_DEBUG_CALL;
		_num = _Total++;
		if (_Total > _Max) _Max = _Total;
		SLB_DEBUG(1,"Create Instance (total=%d) (Max=%d)", _Total, _Max);
	}

	JamesBond::~JamesBond()
	{
		SLB_DEBUG_CALL;
		_Total--;
	}

	std::string JamesBond::speak() const
	{
		std::stringstream s;
		s << "I'm Bond, James Bond (number... mmm..." << _num << ")";
		return s.str();
	}



	void wrapper()
	{
		SLB_DEBUG_CALL;
		SLB_DEBUG(1, "--> Loading unit_007 wrapper -->");

		struct Aux
		{
			static JamesBond::Shared constructor()
			{
				return JamesBond::Shared(new JamesBond());
			}

			static JamesBond* convert( JamesBond::Shared *s )
			{
				return &(**s);
			}
		};
		
		SLB::Class< JamesBond, SLB::Instance::NoCopyNoDestroy >("Unit_007::JamesBond")
			.constructor(&Aux::constructor)
			.set("total", &JamesBond::TOTAL)
			.set("max", &JamesBond::MAX)
			.set("num", &JamesBond::num)
			.set("speak", &JamesBond::speak)
		;

		SLB::Class < JamesBond::Shared >("Unit_007::JamesBond::Shared")
			.convertibleTo(&Aux::convert)
		;

		SLB_DEBUG(1, "<-- Loading unit_007 wrapper <--");
	}


	SLB_ON_LOAD(wrapper)

}
