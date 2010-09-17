SLB.using(SLB.Unit_004)


dog1 = Dog(true)
dog2 = Dog(false)
duck1 = Duck(true)
duck2 = Duck(false)

if SLB.type(duck1) ~= "Unit_004::Duck" then error("Invalid duck class") end
if SLB.type(dog2)  ~= "Unit_004::Dog"  then error("Invalid dog  class") end
if not SLB.isA(dog1, Animal) then error("invalid superclass Animal") end
if not SLB.isA(duck2, Animal) then error("invalid superclass Animal") end
if dog1:bites() == false then error("Dog method") end
if duck2:canFly() == true then error("Duck method") end
if dog2:makeSound() ~= "Woof" then error ("parent method") end
if duck1:makeSound() ~= "cuack!" then error ("parent method") end

