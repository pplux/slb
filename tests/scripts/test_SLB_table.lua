SLB.using(SLB.Unit_SLB)

do
	print("Test Class __index and __newindex of ::Table....")
	Table[5] = "five"
	
	if Table[5] ~= "five" then error("__newindex FAIL") end
	if Table.other ~= nil then error("__index FAIL") end

	Table.farm = "cow"

	if Table.farm ~= "cow" then error("d'oh!") end


end

