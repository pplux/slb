SLB.using(SLB.Unit_002)

v = HClass()

-- equivalent to a link from file...
v:linkFromTable
{
	calc = function (instance, a, b)
		result = a + b
	end,
	get = function ()
		return result
	end
}

v:calc(6,7)

if not v:checkSharedState() then
	error("CheckSharedState failed")
end

print("calc done")
if v:get() ~= 13 then
	error("Result given invalid...")
end

