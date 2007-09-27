#include <SLB/Iterator.hpp>
namespace SLB {

	Iterator::Iterator(IteratorBase *b) : _iterator(b)
	{
	}

	void Iterator::pushImplementation(lua_State *L)
	{
		lua_pushlightuserdata(L, (void*) _iterator );
		lua_pushcclosure( L, Iterator::iterator_call, 1 );
	}

	Iterator::~Iterator()
	{
		delete _iterator;
	}

	int Iterator::iterator_call(lua_State *L)
	{
		IteratorBase *ib = reinterpret_cast<IteratorBase*>( lua_touserdata(L, lua_upvalueindex(1) ) );
		return ib->push(L);
	}

}
