#ifndef __SLB__
#define __SLB__

#include "Manager.hpp"
#include "ClassInfo.hpp"
#include "PushGet.hpp"
#include "Type.hpp"
#include "Table.hpp"
#include "Value.hpp"
#include "Class.hpp"
#include "Enum.hpp"
#include "Hybrid.hpp"
#include "SPP.hpp"

// just for help....
#define SLB_ON_LOAD( FuncName ) \
	SPP_STATIC_BLOCK( FuncName, FuncName(); )

#endif
