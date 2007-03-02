#ifndef __SLB__
#define __SLB__

#include <SLB/Manager.hpp>
#include <SLB/ClassInfo.hpp>
#include <SLB/PushGet.hpp>
#include <SLB/Type.hpp>
#include <SLB/Table.hpp>
#include <SLB/Value.hpp>
#include <SLB/Class.hpp>
#include <SLB/SPP.hpp>

// just for help....
#define SLB_ON_LOAD( FuncName ) \
	SPP_STATIC_BLOCK( FuncName, FuncName(); )

#endif
