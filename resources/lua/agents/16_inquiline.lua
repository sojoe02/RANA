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
Physics = require "ranalib_physics"
Shared = require "ranalib_shared"
Core = require "ranalib_core"

firstStep = true
repulsed = false
scanMultiple = 10
repulsionRange = 0
detectionRange = 0
attractionRange = 0
call_counter = 1
collision_callfrequency = 0

timeToLive = 0
timeToLiveMax = 50

-- Initialization of the agent.
function InitializeAgent()
	
	say("Inquiline Agent #: " .. ID .. " has been initialized")
	
	ColorRed = 200
	ColorBlue = 0
	ColorGreen = 0

	Speed = 4

	repulsionRange = Shared.getNumber("inquiline_repulsion_range")
	detectionRange = Shared.getNumber("inquiline_detection_range")
	attractionRange = Shared.getNumber("inquiline_attraction_range")
	collision_callfrequency = Shared.getNumber("collision_callfrequency")

        timeToLive = Stat.randomMean(5,30)
        say("I start with an expected lifetime of ".. timeToLive .. " seconds")

end

function HandleEvent(Event)

	--avoiding the hosts
	if Event.description == "host" 
		and Physics.calcDistance{x1=Event.X, x2=PositionX, y1=Event.Y, y2=PositionY} < detectionRange 
		then
		--and not repulsed then

		local new_X
		local new_Y 

		--move in opposite direction
		if Event.X > PositionX then
			new_X = PositionX - Stat.randomInteger(repulsionRange/2,repulsionRange)
		else
			new_X = PositionX + Stat.randomInteger(repulsionRange/2,repulsionRange)
		end

		if Event.Y > PositionY then
			new_Y = PositionY - Stat.randomInteger(repulsionRange/2,repulsionRange)

		else
			new_Y = PositionY + Stat.randomInteger(repulsionRange/2,repulsionRange)

		end

		repulsed = true
		timeToLive = timeToLive - Stat.randomMean(1,2)*collision_callfrequency;

		Move.to{x= new_X, y=new_Y}

	elseif Event.description == "inquiline" then

		local new_X
		local new_Y

		--be attracted
		if Event.X > PositionX then
			new_X = PositionX + Stat.randomInteger(attractionRange/2,attractionRange)
		else
			new_X = PositionX - Stat.randomInteger(attractionRange/2,attractionRange)
		end

		if Event.Y > PositionY then
			new_Y = PositionY + Stat.randomInteger(attractionRange/2,attractionRange)

		else
			new_Y = PositionY - Stat.randomInteger(attractionRange/2,attractionRange)

		end

                timeToLive = timeToLive + Stat.randomMean(.1,.2)*collision_callfrequency;

		if timeToLive >= timeToLiveMax then timeToLive = timeToLiveMax end

	end	
end


function TakeStep()

	if firstStep then initStep() end

        call_counter = call_counter +1
        timeToLive = timeToLive -1*STEP_RESOLUTION

	if call_counter >= collision_callfrequency*1/STEP_RESOLUTION then
		Event.emit{description="inquiline"}
		call_counter = 0
	end

	if not Moving then 
		Move.toRandom() 
		repulsed = false
	end

        if timeToLive <= 0 then
            say("Inquiline agent #" .. ID .." died at the old age of  ".. Core.time())
            Event.emit{description="inquiline_death"}
            Agent.removeAgent(ID)
        end

end

function CleanUp()
	say("Inquiline agent has survived with a remaining 'time-to-live' of "..timeToLive.."[s]")
end

function initStep()

	firstStep = false

end



