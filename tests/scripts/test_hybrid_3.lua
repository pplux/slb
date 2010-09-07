print("Using SLB.Unit_002")
SLB.using(SLB.Unit_002)

-- this is a virtual function of HClass.
print("declare: HClass:calc")
function HClass:calc(a,b)
	print("Calling virtual method HClass:calc")
	if SLB.type(self) ~= "Unit_002::HClass" then
		error("Invalid instance @ calc")
	end
	self:perform_calc(a,b)
end

-- this is a virtual function of HClass, like HClass:calc 
print("declare: HClass:get")
function HClass:get()
	print("Calling virtual method HClass:get")
	return result
end

-- this is an extra function of HClass (not in C++)
print("declare: HClass:perform_calc")
function HClass:perform_calc(a, b)
	if SLB.type(self) ~= "Unit_002::HClass" then
		error("Invalid instance @ perform_calc got : "..type(self))
	end
	-- here inner is a global value that must exists once this method
	-- is called.
	print("Trying to call inner --> implemented in subclasses")
	result = self:inner(a,b) + extra
end

-- this is an extra function
print("declare: HClass:setExtra")
function HClass:setExtra(v)
	if SLB.type(self) ~= "Unit_002::HClass" then
		error("Invalid instance @ extra : "..type(self))
	end
	extra = v
end

-- this is an extra function, and should be overriden
print("declare: HClass:inner")
function HClass:inner(a,b)
	error("This method should be implemented in inherited classes")
end

print("Now, add new 'classes' ---------------------------------------------")

print("declare: HClass.Type1:inner")
function HClass.Type1:inner(a,b)
	return a+self:other(b)
end

print("declare: HClass.Type1:other")
function HClass.Type1:other(a)
	extra = extra + 1
	return a*2
end


print("declare: HClass.Type1:get")
-- This implements a C++-virtual function, overriding parent
-- implementation.
function HClass.Type1:get(a)
	return result*2
end

print("---Now Type2... easier class -------------------------------------")
print("declare: HClass.Type2:inner")
function HClass.Type2:inner(a,b)
	return a-b
end

print("---Create Instances ----------------------------------------------")

print("obj1 = HClass.Type1")
obj1 = HClass.Type1()
print("obj1 = HClass.Type2")
obj2 = HClass.Type2()

if SLB.type(obj1) ~= "Unit_002::HClass" then error("Invalid class") end
if SLB.type(obj2) ~= "Unit_002::HClass" then error("Invalid class") end

print("---TEST-----------------------------------------------------------")
print("obj1:setExtra(3)")  obj1:setExtra(3)
print("obj1:calc(3,4)")    obj1:calc(3,4)
print("obj2:setExtra(3)")  obj2:setExtra(6)
print("obj2:calc(3,4)")    obj2:calc(6,8)

print("obj1:get()")
r = obj1:get()
if r ~= 30 then error("Error in calculation = "..r) end

print("obj2:get()")
r = obj2:get()
if r ~= 4 then error("Error in calculation = "..r) end
