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
Stat = require "ranalib_statistic"
Move = require "ranalib_movement"
Collision = require "ranalib_collision"
Utility = require "ranalib_utility"
-- Initialization of the agent.
function initializeAgent()
	say("Agent #: " .. ID .. " has been initialized")

	--Moving = true
	--DestinationX = 1
	--DestinationY = 1
	--Speed = 40
	--GridMove = true
	--Moving = true
	--
	--Collision.updatePosition(20,20)
end


function takeStep()

	--if Moving == false then
		
	--	local x = Stat.randomInteger(1, ENV_WIDTH)
	--	local y = Stat.randomInteger(1, ENV_HEIGHT)		

		--Move.to{x=x, y=y}
		--
	
		
	--end

	--move to 20,20 with  collision detection.
	Collision.updatePosition(20,20)
	if ID == 1 then
		positionTable = {}
		positionTable = Collision.checkPosition(PositionX, PositionY)
		say(Utility.serializeTable(positionTable))
	end
end

function cleanUp()
	
	if ID == 1 then
		positionTable = {}
		positionTable = Collision.checkPosition(PositionX, PositionY)

		say("agent #"..ID.." has these collisions...")
		say(Utility.serializeTable(positionTable))
	 	
		--for i = 1, #positionTable do
		--	say(positionTable[i])
		--end
	end
end

