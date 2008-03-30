SLB.using(SLB.Unit_002)

v = HClass()

v:link
{
	SLB = SLB,
	print = print,
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


print("Now, add new functions to the object-----------------------------")

function HClass:perform_calc(a, b)
	if SLB.type(self) ~= "Unit_002::HClass" then
		error("Invalid instance @ perform_calc got : "..type(self))
	end
	result = a + b + extra
end

function HClass:setExtra(v)
	if SLB.type(self) ~= "Unit_002::HClass" then
		error("Invalid instance @ extra : "..type(self))
	end
	
	extra = v
end

print("---TEST-----------------------------------------------------------")

print("v:setExtra(3)")  v:setExtra(3)
print("v:calc(3,4)")    v:calc(3,4)

print("v:get()")
r = v:get()
if r ~= 10 then error("Error in calculation = "..r) end
