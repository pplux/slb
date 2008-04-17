#ifndef __UNIT_SLB__
#define __UNIT_SLB__

#include <SLB/SLB.hpp>
namespace Unit_SLB {

class Table {
public:
	Table();
	~Table();
	static int class__index(lua_State *L);
	static int class__newindex(lua_State *L);

private:
	Table(const Table&);
	Table& operator==(const Table&);

};

} // end of unit SLB

#endif
