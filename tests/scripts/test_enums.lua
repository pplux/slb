SLB.using(SLB.Unit_006)


t = Test()
print("---------------------------------------")
print("---------------------------------------")
print("---------------------------------------")
print(Test.Enum.A)
print("---------------------------------------")
print("---------------------------------------")
print("---------------------------------------")
print("---------------------------------------")
if not t:equal(Test.Enum.A) then error("Invalid initialization") end
if not t:isA() then error("invalid initialization(2)") end

t:set(Test.Enum.C)
if not t:equal(Test.Enum.C) then error("Invalid set C") end
if not t:isC() then error("Invalid set C(2)") end
if t:equal(Test.Enum.A) then error("Invalid C==A") end
if t:isA() then error("Invalid C==A (2)") end
if t:equal(Test.Enum.B) then error("Invalid C==B") end
if t:isB() then error("Invalid C==B (2)") end

local x = t:get()

t:set(Test.Enum.B)
if not t:equal(Test.Enum.B) then error("Invalid set B") end
if not t:isB() then error("Invalid set B(2)") end
if t:equal(Test.Enum.A) then error("Invalid B==A") end
if t:isA() then error("Invalid B==A (2)") end
if t:equal(Test.Enum.C) then error("Invalid B==C") end
if t:isC() then error("Invalid B==C (2)") end

t:set(x) -- previous value
if not t:equal(Test.Enum.C) then error("Invalid set C") end
if not t:isC() then error("Invalid set C(2)") end
if t:equal(Test.Enum.A) then error("Invalid C==A") end
if t:isA() then error("Invalid C==A (2)") end
if t:equal(Test.Enum.B) then error("Invalid C==B") end
if t:isB() then error("Invalid C==B (2)") end

