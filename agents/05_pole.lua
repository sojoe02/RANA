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
Agent = require "ranalib_agent"

-- Init of the lua frog, function called upon initilization of the LUA auton.
function initializeAgent()

	say("Pole agent #: " .. ID .. " has been initialized")

	Agent.changeColor{r=255}


end

function handleEvent(sourceX, sourceY, sourceID, eventDescription, eventTable)

	local event_table = {x=sourceX, y=sourceY}
	
	Event.emit{targetID=sourceID, speed=343, description="echo", table=event_table} 

end

function cleanUp()
	l_debug("Agent #: " .. ID .. " is done\n")
end

function ProcessEvent(data)

	local A = 10


	--local denom = math.sqrt(data.sX*data.sX+data.sY*data.sY) * math.sqrt(data.pY*data.pY+data.pX*data.pX)

	--local angle = 10

	--if denom > 0 then 
	--
	--local scalar = data.sX*data.pX+data.sY*data.pY
	--angle = math.acos(scalar)

	--say(scalar .." : " .. angle)
	angle = math.abs(math.atan2(data.sX-data.table.x, data.sY-data.table.y) - math.atan2(data.sX-data.pX, data.sY-data.pY))
	--angle2 = math.abs(math.atan2(PositionX, PositionY) - math.atan2(data.pX-PositionX, data.pY-PositionY))

	--angle = math.abs(angle1 - angle2)

	--say(angle)


	
		--angle = math.acos((data.sX*data.pX+data.sY*data.pY)/denom)
	--end

	local Ae = 1/(1+angle * A)

	--agentSay(angle)

	return Ae, 0.2

end

