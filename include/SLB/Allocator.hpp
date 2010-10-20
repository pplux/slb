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

/*
   Based on Steven Lavavej's "Mallocator"
   http://blogs.msdn.com/b/vcblog/archive/2008/08/28/the-mallocator.aspx

   This is a stateless global allocator that redirects all memory 
   allocation to user supplied "malloc" and "free" functions.
   The default implementation actually uses the global malloc and free
   functions.  The user supplied functions must be registered before
   using any SLB component.
*/
#ifndef __SLB_ALLOCATOR__
#define __SLB_ALLOCATOR__

#include <stddef.h>  // Required for size_t and ptrdiff_t
#include <new>       // Required for placement new
#include <stdexcept> // Required for std::length_error

#include "Export.hpp"

namespace SLB
{
	typedef void* (*MallocFn)(size_t);
	typedef void (*FreeFn)(void*);

	SLB_EXPORT void SetMemoryManagement(MallocFn, FreeFn);

	SLB_EXPORT void* Malloc(size_t);
	SLB_EXPORT void Free(void*);

	//Generic stateless allocator that uses the SLB::Malloc and 
	// SLB::Free functions for memory management
	template <typename T>
	class Allocator
	{
	public:
		typedef T * pointer;
		typedef const T * const_pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef T value_type;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		/*
		T * address(T& r) const 
		{
			return &r;
		}
		*/

		const T * address(const T& s) const 
		{
			return &s;
		}


		size_t max_size() const 
		{
			// The following has been carefully written to be independent of
			// the definition of size_t and to avoid signed/unsigned warnings.
			return (static_cast<size_t>(0) - static_cast<size_t>(1)) / sizeof(T);
		}

		template <typename U> struct rebind 
		{
			typedef Allocator<U> other;
		};

		bool operator!=(const Allocator& other) const
		{
			return !(*this == other);
		}

		void construct(T * const p, const T& t) const 
		{
			void * const pv = static_cast<void *>(p);
			new (pv) T(t);
		}

		void destroy(T * const p) const
		{
	#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4100) // unreferenced formal parameter
	#endif
			p->~T();
	#ifdef _MSC_VER
	#pragma warning(pop)
	#endif
		}

		bool operator==(const Allocator& other) const 
		{
			return true;
		}

		Allocator() { }
		Allocator(const Allocator&) { }
		template <typename U>
		Allocator(const Allocator<U>&) { }
		~Allocator() { }

		T * allocate(const size_t n) const
		{
			if (n == 0)
			{
				return NULL;
			}

			if (n > max_size())
			{
				throw std::length_error("Allocator<T>::allocate() - Integer overflow.");
			}

			void * const pv = Malloc(n * sizeof(T));
			if (pv == NULL) 
			{
				throw std::bad_alloc();
			}

			return static_cast<T *>(pv);
		}


		void deallocate(T * const p, const size_t n) const 
		{
			Free(p);
		}

		template <typename U> T * allocate(const size_t n, const U * /* const hint */) const 
		{
			return allocate(n);
		}

	private:
		Allocator& operator=(const Allocator&);
	};

	template <typename T>
	T* AllocatorNew()
	{
		T* ptr = Allocator<T>().allocate(1);
		new ((void*)ptr) T();
		return ptr;
	}

	template <typename T>
	T* AllocatorNew(const T& rhs)
	{
		T* ptr = Allocator<T>().allocate(1);
		Allocator<T>().construct(ptr, rhs);
		return ptr;
	}

	template <typename T, typename U>
	T* AllocatorNew(const U& i)
	{
		T* ptr = Allocator<T>().allocate(1);
		new ((void*)ptr) T(i);
		return ptr;
	}

	template <typename T, typename U, typename V>
	T* AllocatorNew(const U& i, const V& j)
	{
		T* ptr = Allocator<T>().allocate(1);
		new ((void*)ptr) T(i, j);
		return ptr;
	}

	/*
	template <typename T, typename U, typename V, typename W>
	T* AllocatorNew(const U& i, const V& j, const W& k)
	{
		T* ptr = Allocator<T>().allocate(1);
		new ((void*)ptr) T(i, j, k);
		return ptr;
	}
	*/

	template <typename T, typename U>
	void AllocatorNewDeduced(T*& ptr, const U& i)
	{
		ptr = AllocatorNew<T,U>(i);
	}

	template <typename T>
	void AllocatorDelete(T* ptr)
	{
		if (ptr)
		{
			Allocator<T>().destroy(ptr);
			Allocator<T>().deallocate(ptr, 1);
		}
	}
}
#endif
