#ifndef __SLB_OBJECT__
#define __SLB_OBJECT__

#include <assert.h>
#include "Export.hpp"

struct lua_State;

namespace SLB
{
	class SLB_EXPORT Object 
	{
	public:
		unsigned int referenceCount() const { return _refCounter; }
		void ref();
		void unref();

		void push(lua_State *L);

	protected:

		Object();
		virtual ~Object();

		virtual void pushImplementation(lua_State *) = 0;
		virtual void onGarbageCollection(lua_State *) {}
		const char* getID();

	private:
		void initialize(lua_State *);
		static int GC_callback(lua_State *);
		unsigned int _refCounter;

		
		Object( const Object &slbo);
		Object& operator=( const Object &slbo);
	};

	// ------------------------------------------------------------
	// ------------------------------------------------------------
	// ------------------------------------------------------------
		
	inline void Object::ref()
	{
		++_refCounter;
	}

	inline void Object::unref()
	{
		assert(_refCounter > 0);
		--_refCounter; 
		if (_refCounter == 0) delete this;
	}

} //end of SLB namespace

#endif
