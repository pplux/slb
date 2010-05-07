/*
    SLB - Simple Lua Binder
    Copyright (C) 2007 Jose L. Hidalgo Valiño (PpluX)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

	Jose L. Hidalgo (www.pplux.com)
	pplux@pplux.com
*/

#ifndef __SLB_MUTEX__
#define __SLB_MUTEX__

#ifdef SLB_NO_THREAD_SAFE
	namespace SLB { struct MutexData {}; }
#else // SLB_NO_THREAD_SAFE
	#ifdef WIN32

	#else
		#include <pthread.h>	
		namespace SLB { typedef pthread_mutex_t MutexData; }
	#endif
#endif //SLB_NO_THREAD_SAFE

namespace SLB
{

	/// Multiplatform Mutex abstraction, needed to ensure thread safe of
	/// some algorithms. You can turn off Mutexes by compileing SLB with
	/// SLB_NO_THREAD_SAFE

	struct Mutex
	{
		Mutex();
		~Mutex();
		void lock();
		void unlock();
		bool trylock();
	private:
		MutexData _m;
	};

	struct CriticalSection
	{
		CriticalSection(Mutex *m) : _m(m)
		{
			if (_m) _m->lock();
		}
		~CriticalSection()
		{
			if (_m) _m->unlock();
		}
	private:
		Mutex *_m;
	};

	struct ActiveWaitCriticalSection
	{
		ActiveWaitCriticalSection(Mutex *m) : _m(m)
		{
			if (_m) while(_m->trylock() == false) {/*try again*/};
		}
		~ActiveWaitCriticalSection()
		{
			if (_m) _m->unlock();
		}
	private:
		Mutex *_m;
	};


#ifdef SLB_NO_THREAD_SAFE
	inline Mutex::Mutex() {}
	inline Mutex::~Mutex() {}
	inline void Mutex::lock(){}
	inline void Mutex::unlock() {}
#else // SLB_NO_THREAD_SAFE
#ifdef WIN32
#else
	// PTHREADS implementation...
	inline Mutex::Mutex()
	{
		pthread_mutex_init(&_m,0);
	}
	inline Mutex::~Mutex()
	{
		pthread_mutex_destroy(&_m);
	}
	inline void Mutex::lock()
	{
		pthread_mutex_lock(&_m);
	}
	inline void Mutex::unlock()
	{
		pthread_mutex_unlock(&_m);
	}
	inline bool Mutex::trylock()
	{
		return( pthread_mutex_trylock(&_m) == 0) ;
	}
#endif // WIN32


#endif // SLB_NO_THREAD_SAFE


} // end of SLB's namespace

#endif
