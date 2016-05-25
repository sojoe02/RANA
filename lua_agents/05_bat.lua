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

above_count = 1
below_count = 1
speed = 10
Displacement = 1

-- Init of the lua frog, function called upon initilization of the LUA auton.
function initializeAgent()

	l_debug("Agent #: " .. ID .. " has been initialized") 

	PositionY = ENV_HEIGHT/2
	PositionX = 0

	l_debug(ENV_HEIGHT)
end

function handleEvent(sourceX, sourceY, sourceID, eventDescription, eventTable)
	
	if sourceY < PositionY  then
		above_count = above_count + 1 
	else
		below_count = below_count + 1
	end 

	Displacement = above_count/below_count

end

function takeStep()


	if l_getMersenneInteger(1,2000) <= 1 then
	 	Event.emit{speed=343}
	end

	--movement
	PositionX = PositionX + speed * STEP_RESOLUTION

	if Displacement > 1 and PositionY > 70 then
		PositionY = PositionY - speed/2 * STEP_RESOLUTION
	end

		
	if PositionX > ENV_WIDTH then 
		PositionX = 0 
		PositionY = ENV_HEIGHT/2
	end

end

function cleanUp()
	l_debug("Agent #: " .. ID .. " is done\n")
end

function processEventFunction(sourceX, sourceY, posX, posY, time, eventTable)

	return 2

end
