#ifndef __UNIT_003__
#define __UNIT_003__

#include <map>
#include <SLB/Debug.hpp>

namespace Unit_003 {

	template<class Key, class Value>
	struct  MapWrapper : public std::map<Key, Value>
	{
		Value get(Key k)
		{
			SLB_DEBUG_CALL;
			SLB_DEBUG(2, "map::GET [%s->%s]", typeid(Key).name(), typeid(Value).name())
			return (*this)[k];
		}
		void set(Key k, Value v)
		{
			SLB_DEBUG_CALL;
			SLB_DEBUG(2, "map::SET [%s->%s]", typeid(Key).name(), typeid(Value).name())
			(*this)[k] = v; 
		}
	};

	typedef MapWrapper<const char *, int> MapStringInt;
	typedef MapWrapper<int, const char *> MapIntString;


} // end of unit 003

#endif
