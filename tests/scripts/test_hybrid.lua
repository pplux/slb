SLB.using(SLB.Unit_002)

v = HClass()

-- equivalent to a link from file...
if not v:linkFromMemory[[
print("Loading hybrid members...")
function calc(instance, a, b)
	result = a + b
end
function get()
	return result
end
]] then
	error("Error at linking...")
end

if calc ~= nil then
	error("Hybrid has changed GLOBAL state")
end

v:calc(6,7)
print("calc done")
if v:get() ~= 13 then
	error("Result given invalid...")
end

if not v:checkOwnState() then
	error("CheckOwnState failed")
end

