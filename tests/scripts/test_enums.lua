SLB.using(SLB.Unit_006)


t = Test()
if not t:equal(Test.Enum.A) then error("Invalid initialization") end

t:set(Test.Enum.C)
if not t:equal(Test.Enum.C) then error("Invalid set C") end
if t:equal(Test.Enum.A) then error("Invalid C==A") end
if t:equal(Test.Enum.B) then error("Invalid C==B") end

local x = t:get()

t:set(Test.Enum.B)
if not t:equal(Test.Enum.B) then error("Invalid set B") end
if t:equal(Test.Enum.A) then error("Invalid B==A") end
if t:equal(Test.Enum.C) then error("Invalid B==C") end

t:set(x) -- previous value
if not t:equal(Test.Enum.C) then error("Invalid set C") end
if t:equal(Test.Enum.A) then error("Invalid C==A") end
if t:equal(Test.Enum.B) then error("Invalid C==B") end
