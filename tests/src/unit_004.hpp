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


#ifndef __UNIT_004__
#define __UNIT_004__

#include <map>
#include <SLB/Debug.hpp>

namespace Unit_004 {

	class Animal
	{
	public:
		virtual const char* makeSound() const = 0;
		virtual ~Animal() {}
	};

	class Duck : public virtual Animal
	{
	public:
		Duck(bool f) : _fly(f) {}
		virtual bool canFly() const { return _fly; }
		const char *makeSound() const { return "cuack!"; }
	protected:
		bool _fly;
	};

	class Dog : public virtual Animal
	{
	public:
		Dog(bool b) : _bites (b) {}
		virtual bool bites() const {  return _bites; }
		const char *makeSound() const { return "Woof"; }
	protected:
		bool _bites;
	};


} // end of unit 004

#endif
