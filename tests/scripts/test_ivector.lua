SLB.using(SLB.Unit_001)

v = iVector() -- defined as std::vector< Element > 
print(v)
v:push_back( 6 )
v:push_back( 36 )
v:push_back( 129 )

-- get is wrapped to operator[]
if not v:get(0) == 6 then error("Invalid GET[0]") end
if not v:get(1) == 36 then error("Invalid GET[1]") end
if not v:get(2) == 129 then error("Invalid GET[2]") end

-- todo make mor tests
