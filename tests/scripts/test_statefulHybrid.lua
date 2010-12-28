SLB.using(SLB.Unit_005)

local sfh = SFH()
print("-----------------------------------")
sfh:doString(
[[
	SLB.using(SLB.Unit_005)
	print('hi from inside a Script')
	function SFH:calc(v)
		r = (r or 0) + v
		return r
	end
]],"example")
print("-----------------------------------")
local a = sfh:calc(5)
if a ~= 5 then error("error calc 1") end
local a = sfh:calc(-10)
if a ~= -5 then error("error calc 2") end
for i=1,100 do a = sfh:calc(1) end
if a ~=95 then error("error calc 3") end
