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
attractionRange = 30
repulsionRange =6
counter = 1

edgethresshold = 30

avoiding = true
adjusting = false
adjustvalue = 0
adjustmodifier = 40
adjustrate = 2
adjustmodulator = 1

-- Initialization of the agent.
function InitializeAgent()
	
	say("Agent #: " .. ID .. " has been initialized")
	say(Stat.randomInteger(10,30))
	--Angle =math.atan2(ENV_HEIGHT/2-PositionY, ENV_WIDTH/2-PositionX)*57.2958
	--
	adjustmodifier = Stat.randomInteger(10,30)

	Speed = 40
	GridMove = true
	Moving = true

	Move.byAngle(Stat.randomInteger(1,360))
	--Move.byAngle(120)
	--Angle = Stat.randomInteger(1,360)

	--say(Angle)

end

function HandleEvent(event)

	--determine relevance
	--
	--
	if event.description == "angle" then
		local distance = RanaMath.calcDistance{x1=PositionX, x2=event.X, y1=PositionY, y2=event.Y} 
		
		--say(distance)


			--local angle = RanaMath.calcAngle{x1=PositionX, x2=event.X, y1=PositionY, y2=event.Y}
		--say(angle)

		if distance < attractionRange and distance > repulsionRange  and not avoiding then

	--		say(distance)

			local angle = RanaMath.calcAngle{x1=PositionX, x2=event.X, y1=PositionY, y2=event.Y}

		--	say(angle)

			if math.abs(Angle-angle) >=180 then
				if math.abs(Angle - event.table.angle) >= 45 then
					
				if Angle > event.table.angle then

						Move.byAngle(Angle-10)

					elseif Angle < event.table.angle then 
						Move.byAngle(Angle+10)
					end

				else

					--Move.byAngle(event.table.angle)

				end
			end
				--Move.byAngle(event.table.angle)

				--Agent.changeColor{g=255}

				--Event.emit{targetID = event.ID}

			--else

				--Agent.changeColor{r=255}

			--end

		else 
			--Agent.changeColor{r=255}
		end

	else
		Agent.changeColor{r=255}

	end






end

function TakeStep()


	if adjusting then
		adjust(adjustmodulator)
	end


	if not Moving then
		--Move.toRandom()
	end

	if not adjusting then

		if Angle > 270 and Angle <= 360 then
			if PositionY < 0 + edgethresshold then
				--Move.byAngle(Angle+.3)
				adjusting = true
				adjust()
				adjustmodulator = 1
			end
			if PositionX > ENV_WIDTH-edgethresshold then
				adjusting = true
				--Move.byAngle(Angle-.3)
				adjust()
				adjustmodulator = -1
			end
		end


		if Angle > 0 and Angle <90 then

			if PositionX > ENV_WIDTH-edgethresshold then
				--Move.byAngle(Angle+.3)
				adjusting = true
				adjustmodulator = 1
			end
				if PositionY > ENV_HEIGHT-edgethresshold  then
				--Move.byAngle(Angle-.3)
				adjusting = true
				adjustmodulator = -1
			end

		end

		if Angle >= 90 and Angle <=180 then

			if PositionX < 0+edgethresshold then
				adjusting = true

				adjustmodulator = -1
			end	--Move.byAngle(Angle-.3)

			if PositionY > ENV_HEIGHT-edgethresshold then
				--				Move.byAngle(Angle+.3)
				adjusting = true
				adjustmodulator = 1
			end

		end

		if Angle >180 and Angle <=270 then

			if PositionX < 0+edgethresshold then
				--Move.byAngle(Angle+.3)
				adjusting = true
				adjustmodulator = 1
			end
				if PositionY < 0+edgethresshold then
				--Move.byAngle(Angle-.3)
				adjusting = true
				adjustmodulator = -1
			end

		end






		if Stat.randomInteger(1,500) == 1 then

			local table = {angle=Angle}

			Event.emit{table=table, description="angle"}


		end

		if Stat.randomInteger(1, scanMultiple) == 1 then 
			table = Collision.radialCollisionScan(repulsionRange)

			if table ~= nil then

				Agent.changeColor{r=255}



				Move.byAngle(Angle+Stat.randomInteger(-30,30))

				--
				---set a random destination modifier
				--	local destX = Stat.randomInteger(1,5)
				--	local destY = Stat.randomInteger(1,5)

				--	--get a valid random entry in the table
				--	local entry = Stat.randomInteger(1,#table)

				--	-- retrieve any random colliding agent positon in the table.
				--	-- and set a new destination accordingly.
				--	local rand = Stat.randomInteger(0,1)

				--	--				if RanaMath.calcAngle{x1=PositionX, x2=table[entry].posX, y1= table[entry].posY} - then

				--	if rand == 1 then
				--		if table[entry].posX >= PositionX then 
				--			destX = -destX
				--		end
				--		if table[entry].posY > PositionY then
				--			destY = -destY
				--		end
				--	else 
				--		if table[entry].posX > PositionX then 
				--			destX = -destX
				--		end
				--		if table[entry].posY >= PositionY then
				--			destY = -destY
				--		end

				--	end

				--	-- set the new destination and move there
				--	Move.to{x=PositionX+destX, y=PositionY+destY}
				--	--

				--Angle =math.atan2(DestinationY-PositionY, DestinationX-PositionX)*57.2958

				

				avoiding = true

			else
				--scanMultiple = scanMultiple * 1,1

				Agent.changeColor{b=255, r=255, g=255}

				avoiding = false

				--				Agent.changeColor{b=0,r=255}

			end

		end

		counter = counter +1
	end

end

function adjust(modifier)

	if adjusting and adjustvalue < adjustmodifier then
		adjustvalue = adjustvalue + adjustrate

		Move.byAngle(Angle+adjustrate*adjustmodulator)

	else 
		adjusting = false
		adjustvalue = 0

	end

end
