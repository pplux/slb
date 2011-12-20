SLB.using(SLB.Unit_008)

p = POD()
p.i = 5
p.d = 6.7
p.f = 7.8
p.string = "hello"
p.next = POD()

pn = p.next
pn.i = p.i +1
pn.d = p.d +1
pn.f = p.f +1
pn.string = p.string .. "+1"
pn.next = nil


-- checks ...
if not p.i == 5               then error("check01") end
if not p.d == 6.7             then error("check02") end
if not p.f == 7.8             then error("check03") end
if not p.string == "hello"    then error("check04") end
if not p.next == pn           then error("check05") end
if not pn.i == 6              then error("check06") end
if not pn.d == 7.7            then error("check07") end
if not pn.f == 8.8            then error("check08") end
if not pn.string == "hello+1" then error("check09") end
if not pn.next == nil         then error("check10") end

-- modify content
pn.i = 1
pn.d = 2
pn.f = 3
pn.string = "bye"
pn.next = pn

-- more checks
if not pn.i == 1          then error("check06") end
if not pn.d == 2          then error("check07") end
if not pn.f == 3          then error("check08") end
if not pn.string == "bye" then error("check09") end
if not pn.next == pn      then error("check10") end
