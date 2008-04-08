SLB.using(SLB.Unit_002)

-- previous to any instance
function HClass:calc(a,b)
	result = a+b
end

-- create one instance
v = HClass()

-- another function:
function HClass:get()
	return result 
end

v:calc(6,7)

if not v:checkSharedState() then
	error("CheckSharedState failed")
end

print("calc done")
if v:get() ~= 13 then
	error("Result given invalid...")
end

print("Cheking global state")
if result ~= nil then
	print("Result == ", result,"<-")
	error("Hybrid class has interfered global state.")
end
