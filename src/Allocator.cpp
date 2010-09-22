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

#include <SLB/Allocator.hpp>

namespace
{
	SLB::MallocFn s_mallocFn = NULL;
	SLB::FreeFn s_freeFn = NULL;
}

namespace SLB
{
	SLB_EXPORT void SetMemoryManagement(MallocFn mallocFn, FreeFn freeFn)
	{
		s_mallocFn = mallocFn;
		s_freeFn = freeFn;
	}

	SLB_EXPORT void* Malloc(size_t sz)
	{
		if (s_mallocFn)
		{
			return (s_mallocFn)(sz);
		}

		return malloc(sz);
	}

	SLB_EXPORT void Free(void* ptr)
	{
		if (s_freeFn)
		{
			(s_freeFn)(ptr);
		}
		else
		{
			free(ptr);
		}
	}
}
