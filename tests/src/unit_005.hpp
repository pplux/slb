#ifndef __UNIT_005__
#define __UNIT_005__

#include <SLB/SLB.hpp>
namespace Unit_005 {

class SFH : public SLB::StatefulHybrid<SFH>
{
public:
	SFH();
	~SFH();
	HYBRID_method_1(calc,int,int);
protected:
};



} // end of unit SLB

#endif
