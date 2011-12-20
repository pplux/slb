SLB.using(SLB.Unit_001)

e = Element("test")
if e:label() == "test"  then print("Correct read") end
e:setLabel("test2")
if e:label() == "test2" then print("Correct write") end
