SLB.using(SLB.Unit_002)


function createObj()
	local obj = HClass()
	obj:link
	{
		-- this instances do not have shared global state, you need to
		-- pass values of those objects they might have.
		SLB = SLB,
		print = print,

		-- inner functions here can not be accessed from outside
		inner = function(a,b)
			return a + b
		end,

		-- some inner functions are used as Hybrid-methods, here
		-- is where C++ classes look for implemented functions
		calc = function (self, a, b)
			if SLB.type(self) ~= "Unit_002::HClass" then
				error("Invalid instance @ calc")
			end
			self:perform_calc(a,b)
		end,

		get = function (self)
			return result
		end
	}
	return obj
end


print("Now, add new functions to the class-----------------------------")

function HClass:perform_calc(a, b)
	if SLB.type(self) ~= "Unit_002::HClass" then
		error("Invalid instance @ perform_calc got : "..type(self))
	end
	-- here inner is a global value that must exists once this method
	-- is called.
	result = inner(a,b) + self:inner2(extra)
end

function HClass:setExtra(v)
	if SLB.type(self) ~= "Unit_002::HClass" then
		error("Invalid instance @ extra : "..type(self))
	end
	
	extra = v
end

print("---Create Instances ----------------------------------------------")
obj1 = createObj()
obj2 = createObj()
print("Now, add functions to instances-----------------------------------")

function obj1:inner2(v)
	return v+1
end

function obj2:inner2(v)
	return v+2
end

if obj1.inner2 == nil then error("Setting inner2 func on obj1") end
if obj2.inner2 == nil then error("Setting inner2 func on obj2") end

print("---TEST-----------------------------------------------------------")
print("obj1:setExtra(3)")  obj1:setExtra(3)
print("obj1:calc(3,4)")    obj1:calc(3,4)
print("obj2:setExtra(3)")  obj2:setExtra(6)
print("obj2:calc(3,4)")    obj2:calc(6,8)

print("obj1:get()")
r = obj1:get()
if r ~= 11 then error("Error in calculation = "..r) end

print("obj2:get()")
r = obj2:get()
if r ~= 22 then error("Error in calculation = "..r) end
