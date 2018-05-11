----begin_license--
--
--Copyright 	2013 	Søren Vissing Jørgensen.
--			2014	Søren Vissing Jørgensen, Center for Bio-Robotics, SDU, MMMI.  
--
--This file is part of RANA.
--
--RANA is free software: you can redistribute it and/or modify
--it under the terms of the GNU General Public License as published by
--the Free Software Foundation, either version 3 of the License, or
--(at your option) any later version.
--
--RANA is distributed in the hope that it will be useful,
--but WITHOUT ANY WARRANTY; without even the implied warranty of
--MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--GNU General Public License for more details.
--
--You should have received a copy of the GNU General Public License
--along with RANA.  If not, see <http://www.gnu.org/licenses/>.
--
----end_license--

-- @Description:
--  This is a microphone agent. When it gets an event, it will remember the time it got it,
--  and the ID of the emitter.

-- data sets
evTable = {}

-- Import Rana lua libraries.
Core	= require "ranalib_core"
Shared  = require "ranalib_shared"

-- Init of the lua frog, function called upon initilization of the LUA auton.
function initializeAgent()

        if ID == 1 then
            PositionX = 0+50
            PositionY = 0+50
        elseif ID == 2 then
            PositionX = 0+50
            PositionY = 1+50
        elseif ID == 3 then
            PositionX = 0+50
            PositionY = 2+50
        elseif ID == 4 then
            PositionX = -1+50
            PositionY = 3+50
        end

        --print("Microphone agent #: " .. ID .. " is being initialized")
        --print(PositionX.." "..PositionY)

end

function handleEvent(sourceX, sourceY, sourceID, eventDescription, eventTable)

        -- write data to the Olevel table:
        local diff = Core.time()-eventDescription
        --print(sourceID.." Sent time: ".. eventDescription.. "\tArrival: "..Core.time().."\tDiff: "..diff)
        table.insert(evTable, sourceID..","..eventDescription..","..Core.time()..","..diff)

end

function cleanUp()

        exNum = Shared.getNumber(1)
        T = Shared.getNumber(2)
        e = Shared.getNumber(3)
        r = Shared.getNumber(4)
        y = Shared.getNumber(5)
        s = Shared.getNumber(6)
        t = Shared.getNumber(7)
        x = Shared.getNumber(8)

        --Write the oscillation data to a csv file.
        --file = io.open("test/test_11_"..Shared.getNumber(20).."_"..Shared.getNumber(19).."/mic/"..ID.."_"..exNum.."_"..T.."_"..e.."_"..r.."_"..y.."_"..s.."_"..t.."_"..x..".csv", "w")
        --for i,v in pairs(evTable) do
        --        file:write(i..","..v.."\n")
        --end
        --file:close()

end

