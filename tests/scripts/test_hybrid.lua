SLB.using(SLB.Unit_002)

-- example of how to use an string to define functions of an Hybrid class
chunk = assert(loadstring[[
print("Loading hybrid members...")

function HClass:calc(a, b)
	result = a + b
end

function HClass:get()
	return result
end

]])()

print("constructor...")
-- that's the constructor:
v = HClass()

v:calc(6,7)
print("calc done")
if v:get() ~= 13 then
	error("Result given invalid...")
end

if not v:checkSharedState() then
	error("CheckSharedState failed")
end

print("Cheking global state")
if result ~= nil then
	print("Result == ", result,"<-")
	error("Hybrid class has interfered global state.")
end

