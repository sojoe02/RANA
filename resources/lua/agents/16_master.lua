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

-- Import Rana lua libraries.
Agent	= require "ranalib_agent"
Shared = require "ranalib_shared"
-- Init of the lua frog, function called upon initilization of the LUA auton.
inquiline_amount = 35
host_amount = 30
inquiline_repulsion_range = 10
inquiline_detection_range = 30
inquiline_attraction_range = 10
collision_callfrequency = .500 

amountOfInquilineKilled = 0

function InitializeAgent()
	-- Add the data collector agent.
  	PositionX = -1
	PositionY = -1

	host_ids = {}
	inquiline_ids = {}

	Shared.storeNumber("inquiline_repulsion_range", inquiline_repulsion_range)
	Shared.storeNumber("inquiline_detection_range", inquiline_detection_range)
	Shared.storeNumber("inquiline_attraction_range", inquiline_attraction_range)
	Shared.storeNumber("collision_callfrequency", collision_callfrequency)

	-- Load up the oscillator agents.
	for i=1 , inquiline_amount do
		local ID = Agent.addAgent("16_inquiline.lua")
		table.insert(inquiline_ids, ID)
	end	

	for i=1, host_amount do
		local ID = Agent.addAgent("16_host.lua")
		table.insert(host_ids, ID)
	end
	
	Shared.storeTable("inquiline_ids", inquiline_IDs) --not used for anything
	Shared.storeTable("host_ids", host_IDs) --not used for anything

end

function HandleEvent(Event)

    if Event.description == "inquiline_death" then

        amountOfInquilineKilled = amountOfInquilineKilled + 1
        --add a new inquiline to replace the one that has died.
        local ID = Agent.addAgent("16_inquiline.lua")


    end

end

function CleanUp()

     shout("Amount of inquilines that has died this time around is : " .. amountOfInquilineKilled)

end
