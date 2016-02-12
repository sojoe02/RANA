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

--The following global values are set via the simulation core:
-- ID -- id of the agent.
-- PositionX --	this agents x position.
-- PositionY -- this agents y position.
-- STEP_RESOLUTION 	-- resolution of steps, in the simulation core.
-- EVENT_RESOLUTION	-- resolution of event distribution.
-- StepMultiple 	-- amount of steps to skip.


-- Import valid Rana lua libraries.
Event = require "ranalib_event"

-- Init of the lua frog, function called upon initilization of the LUA auton.
function initializeAgent()

	l_debug("Agent #: " .. ID .. " has been initialized")

end

function handleEvent(sourceX, sourceY, sourceID, eventDescription, eventTable)
	
	if eventDescription == "ping" then
	
		l_print("Agent: "..ID .." received a ping from: "..sourceID ..", saying: "..eventTable.msg)
		Event.emit{targetID=sourceID, speed=343, description="pong"}

	elseif eventDescription == "pong" then
		
		l_print("Agent: "..ID.." received a pong from agent: ".. sourceID)
	end

end

function takeStep()


	if l_getMersenneInteger(1,1000) <= 1 then
		l_debug("Agent:"..ID.." is emiting ping")
	 	Event.emit{speed=343, description="ping",table={msg="I am agent "..ID}}
	end

end

function cleanUp()
	l_debug("Agent #: " .. ID .. " is done\n")
end

