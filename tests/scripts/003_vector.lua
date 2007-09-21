SLB.using(SLB.Unit_001)

v = Vector() -- defined as std::vector< Element > 
v:push_back( Element("1 - Element") )
v:push_back( Element("2 - Element") )
v:push_back( Element("3 - Element") )

-- get is wrapped to operator[]
if not v:get(0):label() == "1 - Element" then error("Invalid GET[0]") end
if not v:get(1):label() == "2 - Element" then error("Invalid GET[0]") end
if not v:get(2):label() == "3 - Element" then error("Invalid GET[0]") end
if not v:get(3):label() == nil then error("should return null (out of bounds)") end

-- test non-const get
v:get(0):setLabel("new 1")
if not v:get(0):label() == "new 1" then error ("data should be modified, label is", v:get(0):label()) end

-- test const get
element = v:get_const(0)
result = pcall( function() element:setLabel("testing") end)
if result == true then error("get_const should return a const (non mutable) object") end


-- test iterator
local c = 0
for e in v:iterator() do
	c = c +1
	print(e:label())
end

if c ~= 3 then error("incorrect number of elements in vector c = ", c) end
