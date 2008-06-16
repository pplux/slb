#include <SLB/SLB.hpp>
#include "unit_001.hpp"
#include <SLB/Iterator.hpp>

namespace Unit_001 {

	Element::Element(const std::string &label) : _label(label) {}

	const std::string& Element::label() const
	{
		return _label;
	}

	void Element::setLabel(const std::string &label)
	{
		_label = label;
	}

	void wrapper()
	{
		SLB_DEBUG_CALL;
		SLB_DEBUG(1, "--> Loading unit_001 wrapper -->");
		
		SLB::Class<Element>("Unit_001::Element")
			.constructor<const std::string&>()
			.set( "label", &Element::label )
			.set( "setLabel", &Element::setLabel)
		;
		
		typedef std::vector<Element> Vector;
		typedef std::vector<int> iVector;

		SLB::Class< Vector >("Unit_001::Vector")
			.constructor()
			.set("push_back", &Vector::push_back)
#ifdef WIN32
			.set("get", &Vector::operator[] )
// Windows 2008 needs complete specification... :(
//			.const_set("get_const",   &Vector::operator[] )
//			.iterator<Vector::iterator>("iterator", &Vector::begin, &Vector::end )
//			.const_iterator<Vector::const_iterator>("const_iterator", &Vector::begin, &Vector::end )
		;
#else
			.nonconst_set("get",  &Vector::operator[] )
			.const_set("get_const",  &Vector::operator[] )
			.iterator("iterator", &Vector::begin, &Vector::end )
			.const_iterator("const_iterator", &Vector::begin, &Vector::end )
		;
#endif

		SLB::Class< iVector >("Unit_001::iVector")
			.constructor()
			.set("push_back", &iVector::push_back)
#ifdef WIN32
			.set("get", &iVector::operator[] )
// Windows 2008 needs complete specification... :(
//			.const_set("get_const",   &iVector::operator[] )
//			.iterator<iVector::iterator>("iterator", &iVector::begin, &iVector::end )
//			.const_iterator<iVector::const_iterator>("const_iterator", &iVector::begin, &iVector::end )
		;
#else
			.nonconst_set("get",  &iVector::operator[] )
			.const_set("get_const",  &iVector::operator[] )
			.iterator("iterator", &iVector::begin, &iVector::end )
			.const_iterator("const_iterator", &iVector::begin, &iVector::end )
		;
#endif

		SLB_DEBUG(1, "<-- Loading unit_001 wrapper <--");
	}


	SLB_ON_LOAD(wrapper)

}
