#ifndef __UNIT_001__
#define __UNIT_001__

namespace Unit_001 {

class Element
{
public:
	// constructor
	Element(const std::string &label = "nothing" );

	// methods
	const std::string& label() const;
	void setLabel(const std::string &label);
protected:
	std::string _label;
};

} // end of unit 001

#endif
