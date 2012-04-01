SLB.using(SLB.Unit_006)

collectgarbage()
t = Test()
print("---------------------------------------")
print("---------------------------------------")
print("---------------------------------------")
print(Test.Enum.A)
print("---------------------------------------")
print("---------------------------------------")
print("---------------------------------------")
print("---------------------------------------")
collectgarbage()
if not t:equal(Test.Enum.A) then error("Invalid initialization") end
if not t:isA() then error("invalid initialization(2)") end

collectgarbage()

t:set(Test.Enum.C)
if not t:equal(Test.Enum.C) then error("Invalid set C") end
if not t:isC() then error("Invalid set C(2)") end
if t:equal(Test.Enum.A) then error("Invalid C==A") end
if t:isA() then error("Invalid C==A (2)") end
if t:equal(Test.Enum.B) then error("Invalid C==B") end
if t:isB() then error("Invalid C==B (2)") end

collectgarbage()
local x = t:get()
collectgarbage()

t:set(Test.Enum.B)
if not t:equal(Test.Enum.B) then error("Invalid set B") end
if not t:isB() then error("Invalid set B(2)") end
if t:equal(Test.Enum.A) then error("Invalid B==A") end
if t:isA() then error("Invalid B==A (2)") end
if t:equal(Test.Enum.C) then error("Invalid B==C") end
if t:isC() then error("Invalid B==C (2)") end

collectgarbage()
t:set(x) -- previous value
if not t:equal(Test.Enum.C) then error("Invalid set C") end
if not t:isC() then error("Invalid set C(2)") end
if t:equal(Test.Enum.A) then error("Invalid C==A") end
if t:isA() then error("Invalid C==A (2)") end
if t:equal(Test.Enum.B) then error("Invalid C==B") end
if t:isB() then error("Invalid C==B (2)") end

collectgarbage()
-- comparisons in lua
if Test.Enum.C ~= Test.Enum.C then error("Invalid Test.Enum.C == Test.Enum.C") end
if Test.Enum.A == Test.Enum.C then error("Invalid Test.Enum.C == Test.Enum.C") end
-- BUG? -----
local kk = Test.Enum.A
probe("show_all_classes")
local res = t:create(0)
print("CHECK-----")
print(Test.Enum.A)
print("-----------")
print(res)
print("-----------")
if res ~= Test.Enum.A then
    error("Invalid Test.Enum.A == Test.create(0) <- returns A")
end
------------------------
if Test.Enum.B ~= t:create(1) then error("Invalid Test.Enum.B == Test.create(1) <- returns B") end
if Test.Enum.C ~= t:create(2) then error("Invalid Test.Enum.C == Test.create(2) <- returns C") end

