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
