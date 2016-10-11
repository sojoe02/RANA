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
Event	= require "ranalib_event"
Core	= require "ranalib_core"
Stat	= require "ranalib_statistic"
Shared 	= require "ranalib_shared"
Utility = require "ranalib_utility"

function InitializeAgent()
	
	say("Data Collector initialized")

	ids = Shared.getTable("ids")
	agent_table = Shared.getTable("agents")
	population = Shared.getTable("population")
	--say(Utility.serializeTable(agent_table))

end

-- Init of the lua frog, function called upon initilization of the LUA auton.
function HandleEvent(event)

	agent_table[event.ID].call_amount = agent_table[event.ID].call_amount + 1

end


function CleanUp()

	--Write the oscillation data to a csv file.
	--
	for key1,value1 in pairs(population) do
	
		file = io.open("11_overall_stats_"..value1.speed..".csv", "w")
		file:write("id("..value1.speed.."),".. "calls(" ..value1.speed..")\n")
--
--	say(Utility.serializeTable(agent_table))

		for key,value in pairs(ids) do
			if agent_table[value].movement_speed == value1.speed then
				file:write(value ..","..agent_table[value].call_amount .."\n")
			end
		end
	
		file:close()

	end
end

