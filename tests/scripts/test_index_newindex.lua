SLB.using(SLB.Unit_003)

function printTable(t, level)
	level = level or 0
	for k,v in pairs(t) do
		print("("..level..") ["..tostring(k).."] = "..tostring(v))
		if type(v) == "table" then
			printTable(v, level + 1)
		end	
	end
end
map = MapIntString()

print("----------------- map[5] = txt1 ---------------")
map[5] = "txt1"
print("----------------- map[6] = txt1 ---------------")
map[6] = "txt2"

print("-----------------------------------------------")
printTable(debug.getmetatable(map))

print("----------------- map[5] = ? ---------------")
if map[5] ~= "txt1" then error("Invalid key 5 -> "..tostring(map[5])) end

print("----------------- map[6] = ? ---------------")
if map[6] ~= "txt2" then error("Invalid key 6 -> "..tostring(map[6])) end

print("----------------- map[2] = ? ---------------")
if map[2] ~= nil then error("Map should return default value '' -> "..tostring(map[2])) end
