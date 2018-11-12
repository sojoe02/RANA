----begin_license--
--
--Copyright 	2013 - 2016 	Søren Vissing Jørgensen.
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
-- ------------------------------------
-- IMMUTABLES.
-- ------------------------------------
-- ID -- id of the agent.
-- STEP_RESOLUTION 	-- resolution of steps, in the simulation core.
-- EVENT_RESOLUTION	-- resolution of event distribution.
-- ENV_WIDTH -- Width of the environment in meters.
-- ENV_HEIGHT -- Height of the environment in meters.
-- ------------------------------------
-- VARIABLES.
-- ------------------------------------
-- PositionX	 	-- Agents position in the X plane.
-- PositionY	 	-- Agents position in the Y plane.
-- DestinationX 	-- Agents destination in the X plane. 
-- DestinationY 	-- Agents destination in the Y plane.
-- StepMultiple 	-- Amount of steps to skip.
-- Speed 			-- Movement speed of the agent in meters pr. second.
-- Moving 			-- Denotes wether this agent is moving (default = false).
-- GridMove 		-- Is collision detection active (default = false).
-- ------------------------------------

-- Import valid Rana lua libraries.
Event = require "ranalib_event"
Move = require "ranalib_movement"

suspend = 1

-- Init of the lua frog, function called upon initilization of the LUA auton.
function InitializeAgent()
	say("Agent #: " .. ID .. " has been initialized")

	if ID == 2 then suspend = 8*1/STEP_RESOLUTION end

	if ID == 3 then 
		PositionX = 0
		PositionY = 0
		Speed = 5
	
	end

	if ID == 2 then 
		PositionX = ENV_WIDTH/2-4
		PositionY = ENV_HEIGHT/2+0
	end

	if ID == 1 then 
		PositionX = ENV_WIDTH/2
		PositionY = ENV_HEIGHT/2
	end

end

function HandleEvent(event)

	if event.ID == 2 and ID ~= 1 then
		Move.to{x=event.X, y=event.Y}
	end
end

function takeStep()

	suspend = suspend -1


	if suspend == 0 and ID ~= 3 then

		say("Agent:"..ID.." is emiting ping")
	 	Event.emit{speed=343,description="ping",table={msg="I am agent "..ID}}
		suspend = 1/STEP_RESOLUTION

	end

	if suspend == 0 and ID == 3 then

		if ID == 3 then Move.to{x=ENV_WIDTH/2, y=ENV_HEIGHT/2} end

	end


end


