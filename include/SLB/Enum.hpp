#define __SLB_ENUM__

#include "SPP.hpp"
#include "Export.hpp"
#include "Manager.hpp"
#include "FuncCall.hpp"
#include "Value.hpp"
#include "Instance.hpp"
#include <typeinfo>
#include <map>
#include <vector>
#include <string>

struct lua_State;

namespace SLB {

	class Enum {
	public:

		Enum(const char *name);

		Enum &rawSet(const char *name, Object *obj);

		template<typename TValue>
		Enum &set(const char *name, const TValue &obj)
		{ return rawSet(name, (Object*) Value::copy(obj)); }

		template<typename TValue>
		Enum &set_ref(const char *name, TValue obj)
		{ return rawSet(name, Value::ref(obj)); }

		template<typename TValue>
		Enum &set_autoDelete(const char *name, TValue *obj)
		{ return rawSet(name, Value::autoDelete(obj)); }

		#define SLB_REPEAT(N) \
		\
			/* C-functions  */ \
			template<class R SPP_COMMA_IF(N) SPP_ENUM_D(N, class T)> \
			Enum &set(const char *name, R (func)(SPP_ENUM_D(N,T)) ); \

		SPP_MAIN_REPEAT_Z(MAX,SLB_REPEAT)
		#undef SLB_REPEAT

	protected:
		Table *_table;

	};
	
	inline Enum::Enum(const char *name)
	{
		_table = new Table();
		Manager::getInstance().set(name, _table);
	}
	
	inline Enum& Enum::rawSet(const char *name, Object *obj)
	{
		_table->set(name, obj);
		return *this;
	}
	
	#define SLB_REPEAT(N) \
	\
		/* C-functions  */ \
		template<class R SPP_COMMA_IF(N) SPP_ENUM_D(N, class T)> \
		inline Enum& Enum::set(const char *name, R (func)(SPP_ENUM_D(N,T)) ){ \
			return rawSet(name, FuncCall::create(func)); \
		} \

	SPP_MAIN_REPEAT_Z(MAX,SLB_REPEAT)
	#undef SLB_REPEAT
}
