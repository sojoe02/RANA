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

-- Import valid Rana lua libraries.
Event = require "ranalib_event"
Stat = require "ranalib_statistic"
Move = require "ranalib_movement"

-- Initialization of the agent.
function initializeAgent()
	say("Agent #: " .. ID .. " has been initialized")

	if Moving ~= true then
		say("I am not moving".. STEP_RESOLUTION.. ":" .. PositionX)
	end
	--Moving = true
	--DestinationX = 1
	--DestinationY = 1
	--Speed = 3
end


function takeStep()

	if Moving == false then

		say("nothing")
		
		local x = Stat.randomInteger(1, ENV_WIDTH)
		local y = Stat.randomInteger(1, ENV_HEIGHT)		

		Move.to{x=x, y=y, speed=10}
		
	end
end

