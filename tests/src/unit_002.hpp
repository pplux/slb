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


#ifndef __UNIT_002__
#define __UNIT_002__

#include <SLB/Hybrid.hpp>

namespace Unit_002 {

	struct HybridLock : public SLB::ActiveWaitCriticalSection
	{
		HybridLock(SLB::Mutex *m) : SLB::ActiveWaitCriticalSection(m) { lockCalled = true; total++;  }
		~HybridLock() { unlockCalled = true; total--;}

		static bool lockCalled;
		static bool unlockCalled;
		static int total;
	};

	class HClass : public SLB::Hybrid<HClass, HybridLock>
	{
	public:
		HClass();

		/* int get() { return LCall<int>("get"); } */
		HYBRID_method_0(get,int);

		/* void calc(int a, int b) { return LCall<void, int, int>("calc"); } */
		HYBRID_method_2(calc,void,int,int);

		HYBRID_const_method_0(update,void);

		bool checkSharedState(); //< should be true when linking from a lua_table

	private:
		int _total;
	};

} // end of unit 002

#endif
