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
Agent = require "ranalib_agent"
Event = require "ranalib_event"
RanaMath = require "ranalib_math"

scanMultiple = 10
attractionRange = 50
repulsionRange = 10
counter = 1

-- Initialization of the agent.
function InitializeAgent()
	
	say("Agent #: " .. ID .. " has been initialized")

	Move.toRandom()

	--Angle =math.atan2(ENV_HEIGHT/2-PositionY, ENV_WIDTH/2-PositionX)*57.2958

	Speed = 40
	GridMove = true
	Moving = true

	Move.byAngle(181)

	--Angle = Stat.randomInteger(1,360)

	say(Angle)

end

function HandleEvent(event)

	--determine relevance
	--
	local distance = RanaMath.calcDistance{x1=PositionX, x2=event.X, y1=PositionY, y2=event.Y} 
	if distance < attractionRange and distance > repulsionRange then

		if RanaMath.calcAngle{x1=PositionX, x2=event.X, y1=PositionY, y2=event.Y} < 90 then
			Move.byAngle(event.table.angle)
		end
	end
	
		


	
end

function TakeStep()
	

		Agent.changeColor{b=255}

		if not Moving then
			Move.toRandom()
		end


		if Stat.randomInteger(1,1000) == 1 then

			local table = {angle=Angle}

			Event.emit{table=table}


		end

		if counter % scanMultiple == 0 and Stat.randomInteger(1,1000) == 1 then 
			table = Collision.radialCollisionScan(repulsionRange)

			if table ~= nil then

				--set a random destination modifier
				local destX = Stat.randomInteger(1,10)
				local destY = Stat.randomInteger(1,10)

				--get a valid random entry in the table
				local entry = Stat.randomInteger(1,#table)

				-- retrieve any random colliding agent positon in the table.
				-- and set a new destination accordingly.
				local rand = Stat.randomInteger(0,1)

				if rand == 1 then
					if table[entry].posX >= PositionX then 
						destX = -destX
					end
					if table[entry].posY > PositionY then
						destY = -destY
					end
				else 
					if table[entry].posX > PositionX then 
						destX = -destX
					end
					if table[entry].posY >= PositionY then
						destY = -destY
					end

				end

				-- set the new destination and move there
				Move.to{x=PositionX+destX, y=PositionY+destY}
				--

				--Angle =math.atan2(DestinationY-PositionY, DestinationX-PositionX)*57.2958

				scanMultiple = 10

			else
				scanMultiple = scanMultiple * 1,1
	
			end

	end

	counter = counter +1
end


