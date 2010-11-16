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


#ifndef __UNIT_007__
#define __UNIT_007__

#include <SLB/SLB.hpp>
#if defined(_MSC_VER) 
#	include <memory>
#else
#	include <tr1/memory>
#endif


namespace Unit_007 {

	class JamesBond
	{
	public:
		typedef std::tr1::shared_ptr<JamesBond> Shared;

		JamesBond();
		~JamesBond();

		std::string speak() const;
		int num() const { return _num; }

		// Total of instances that are currently alive
		static int TOTAL() { return _Total; }

		// Max num of instances (historic data)
		static int MAX()   { return _Max; }
	private:
		JamesBond(const JamesBond &);
		JamesBond& operator=(const JamesBond &);
		int _num;
		static int _Total;
		static int _Max;
	};


} // end of unit SLB

#endif
