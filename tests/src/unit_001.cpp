/*
    SLB - Simple Lua Binder
    Copyright (C) 2007-2010 Jose L. Hidalgo Valiño (PpluX)

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
	
	Jose L. Hidalgo (www.pplux.com)
	pplux@pplux.com
*/


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
			.const_set<Vector, Vector::const_reference, Vector::size_type>("get_const", &Vector::operator[])
				.iterator<Vector, Vector::iterator>("iterator",&Vector::begin, &Vector::end )
				.const_iterator<Vector, Vector::const_iterator>("const_iterator", &Vector::begin,&Vector::end )
			
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
			.const_set<iVector, iVector::const_reference, iVector::size_type>("get_const", &iVector::operator[])
			.iterator<iVector, iVector::iterator>("iterator",&iVector::begin, &iVector::end )
			.const_iterator<iVector, iVector::const_iterator>("const_iterator", &iVector::begin,&iVector::end )
		;
#else
		// for basic types we do not support returning a reference yet
		// so it is not allowed to return so. 
			.const_set("get",  &iVector::operator[] )
			.const_set("get_const",  &iVector::operator[] )
			.iterator("iterator", &iVector::begin, &iVector::end )
			.const_iterator("const_iterator", &iVector::begin, &iVector::end )
		;
#endif

		SLB_DEBUG(1, "<-- Loading unit_001 wrapper <--");
	}


	SLB_ON_LOAD(wrapper)

}
