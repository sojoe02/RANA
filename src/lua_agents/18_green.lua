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
-- This agent will oscilate between 0 and 1, with a time period. Upon peaking it will emit an event. 
-- and then rapidly fall down towards zero and restart.
--
-- Upon receiving a signal from another oscillator it will, interrupt it's current cycle, reset
-- and after a small pause it will resume a new cycle.
--
-- Data for each period will stored it a table, which upon simulation cleanUp is written
-- to the harddrive.

-- ID -- id of the agent.
-- PositionX --	this agents x position.
-- PositionY -- this agents y position.
-- STEP_RESOLUTION 	-- resolution of steps.
-- EVENT_RESOLUTION 	-- resolution of event distribution.
-- StepMultiple 	-- step resolution multiplier (default = 1).

-- data sets
Olevels = {}
dataFactor = 100
step = 0
iteration = 1

-- Oscillator values:
T = 0.500 -- time period.
e = 0.030 -- period variance with mean of 0.
r = 0.100 -- falltime.
Tt = 0 -- active period targeted time.
Tn = 0 -- active period time
y = 0.05 -- interrupt pause
s = .2 -- Prc(phase response) slope, how quickly the oscillator recover from inhibition (the smaller the faster).
t = 0.060
x = 0.050
yy = 0

pause = false
reset = false
peaked = false

-- Import Rana lua libraries.
Event	= require "ranalib_event"
Core	= require "ranalib_core"
Stat	= require "ranalib_statistic"
Agent 	= require "ranalib_agent"
Utility = require "ranalib_utility"
Shared  = require "ranalib_shared"

-- Init of the lua frog, function called upon initilization of the LUA auton.
function initializeAgent()

        print("Green agent #: " .. ID .. " is being initialized")

        if ID == 5 then
            PositionX = Shared.getNumber(9)+50
            PositionY = Shared.getNumber(10)+50
            print(Shared.getNumber(1).." "..Shared.getNumber(2).." "..Shared.getNumber(3).." "..Shared.getNumber(4).." "..Shared.getNumber(5).." "..Shared.getNumber(6).." "..Shared.getNumber(7).." "..Shared.getNumber(8))
        elseif ID == 6 then
            PositionX = Shared.getNumber(11)+50
            PositionY = Shared.getNumber(12)+50
        elseif ID == 7 then
            PositionX = Shared.getNumber(13)+50
            PositionY = Shared.getNumber(14)+50
        elseif ID == 8 then
            PositionX = Shared.getNumber(15)+50
            PositionY = Shared.getNumber(16)+50
        elseif ID == 9 then
            PositionX = Shared.getNumber(17)+50
            PositionY = Shared.getNumber(18)+50
        end

        exNum = Shared.getNumber(1)
        T = Shared.getNumber(2)
        e = Shared.getNumber(3)
        r = Shared.getNumber(4)
        y = Shared.getNumber(5)
        s = Shared.getNumber(6)
        t = Shared.getNumber(7)
        x = Shared.getNumber(8)

        Tt = T + Stat.randomMean(e,0)
end

function takeStep()

	Tn = Tn + STEP_RESOLUTION

	if pause == true and Tn >= yy then
		table.insert(Olevels, Core.time()..",".. 0)
		pause = false	
	end

	if peaked == false and Tn >= Tt-t then
		table.insert(Olevels, Core.time()..",".. 1 ..",peak")
		peaked = true
	end

	if reset==false and Tn >= r then
		table.insert(Olevels,Core.time()..",".. 0)
		reset = true
	end

        if Tn >= Tt then
                --say("ID: "..ID.." Emitting signal at time: ".. Core.time())
                Event.emit{speed=343,description=Core.time()}
		table.insert(Olevels, Core.time()..",".. (Tn-r)/(Tt) ..",call")
                Tt = T + Stat.randomMean(e, 0)
		Tn = 0
		peaked = false
                reset = false
	end
end

function handleEvent(sourceX, sourceY, sourceID, eventDescription, eventTable)
	
	if Tn >= x then
		-- write data to the Olevel table:
		table.insert(Olevels, Core.time()..","..Tn/Tt..",interrupt")
		table.insert(Olevels, Core.time()..",".. 0)
		--calculate new period
		Tt = Tn * s + Tt
		yy = y+Tn
		if peaked == true then
			Tn = Tt-y
		end
		pause = true
	end

end

function cleanUp()

	--Write the oscillation data to a csv file.
        file = io.open("test/green_"..ID.."_"..exNum.."_"..T.."_"..e.."_"..r.."_"..y.."_"..s.."_"..t.."_"..x..".csv", "w")
        for i,v in pairs(Olevels) do
                file:write(i..","..v.."\n")
        end
        file:close()

end

