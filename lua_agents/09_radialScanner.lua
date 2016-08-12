--begin_license--
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
Collision = require "ranalib_collision"
Map = require "ranalib_map"
Utility = require "ranalib_utility"

-- Initialization of the agent.
function initializeAgent()
	GridMove = true

	if ID == 1 then
		local table = Map.getRadialMask(5)
		local tableString = Utility.serializeTable(table)
		say(tableString)
	end

end

function takeStep()

	local table = Collision.radialCollisionScan(10)
	
	if table then
		
		say("Agent: ".. ID.. " has these collisions")
		
		for i = 1, #table do
			say("\tAgent: "..table[i].id .. " at X:".. table[i].posX .. " Y:" .. table[i].posY) 
		end
		--local tableString = Utility.serializeTable(table)
		--say(tableString)

	end

end

