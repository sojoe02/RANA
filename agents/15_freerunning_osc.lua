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
sim = {}
step = 0
iteration = 1

-- Oscillator values:
T = 0.500 -- time period.
e = 0.030 -- period variance with mean of 0.
r = 0.100 -- falltime.
Tt = 0 -- active period targeted time.
Tn = 0 -- active period time
y = 0.05 -- interrupt pause

peaked = false

-- Import Rana lua libraries.
Event	= require "ranalib_event"
Core	= require "ranalib_core"
Stat	= require "ranalib_statistic"
Agent 	= require "ranalib_agent"
Utility = require "ranalib_utility"

-- Init of the lua frog, function called upon initilization of the LUA auton.
function initializeAgent()
        --l_debug("Oscillator agent #: " .. ID .. " is being initialized")

        sim = Utility.loadTable("simulation", "_parameters.data")

        --positionX = Stat.randomMean(ENV_WIDTH/4,ENV_WIDTH/2)
        --positionY = Stat.randomMean(ENV_HEIGHT/4,ENV_HEIGHT/2)
        --agentSay("Free "..ID.." x "..positionX.." y "..positionY)

	Tt = T + Stat.randomMean(e,0)

        table.insert(Olevels, Core.time()..",".. 0)
end

function takeStep()

	Tn = Tn + STEP_RESOLUTION
	step = step 

	if Tn >= Tt-r and peaked == false then
		Event.emit{description="Signal"}
		table.insert(Olevels, Core.time()..",".. 1)
		peaked = true
	end

	if Tn >= Tt then 
		Tt = T + Stat.randomMean(e, 0)
		Tn = 0
		table.insert(Olevels, Core.time()..",".. 0)
--		l_print("Interrupt Oscillator "..ID.." Emitting signal at time: ".. Core.time().."[s]")
		peaked = false
	end

end

function cleanUp()
        --Write the oscillation data to a csv file.
        if ID <= 4 then --Why?
                file = io.open("test_output/data_free_5_"..iteration.."_"..ID..".csv", "w")
                for i,v in pairs(Olevels) do
                        file:write(i..","..v.."\n")
                end
                file:close()
        end

        Agent.removeAgent(ID)
        --l_debug("Free - Clean up for Agent " .. ID .. " is done")
end
