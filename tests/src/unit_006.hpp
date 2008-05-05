#ifndef __UNIT_006__
#define __UNIT_006__

#include <SLB/SLB.hpp>
namespace Unit_006 {


	class Test
	{
	public:
		enum Enum
		{
			A,
			B,
			C
		};

		Test();
		void set(Enum e);
		Enum get() const;
		bool equal(Enum e) const;
		bool isA() const;
		bool isB() const;
		bool isC() const;
	private:
		Enum _value;
	};


} // end of unit SLB

#endif
