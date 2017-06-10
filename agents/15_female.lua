---begin_license--
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
Agent 	= require "ranalib_agent"
Utility = require "ranalib_utility"

x = 0.060
beta =  x + 0.030
countdown = beta

total_calls = 0
synced_calls = 0
synced = false

-- data sets
sim = {}

function InitializeAgent()
        sim = Utility.loadTable("simulation", "_parameters.data")

	ids = Shared.getTable("ids")
	agent_table = Shared.getTable("agents")

        Agent.changeColor{r=255,b=255}
end

-- Init of the lua frog, function called upon initilization of the LUA auton.
function HandleEvent(event)
	agent_table[event.ID] = agent_table[event.ID] + 1

	if countdown > 0 then
		synced_calls = synced_calls + 1
		if synced == true then 
			synced_calls = synced_calls + 1
			synced = false
		end
		
		--countdown = 0
	else
		--say(synced_calls)
		synced_calls = 0
                --agentSay(event.ID)
		countdown = beta
		synced = true
	end

	total_calls = total_calls + 1
end

function TakeStep()
	if countdown >= 0 then
		countdown = countdown - STEP_RESOLUTION
 	end
end

function CleanUp()
        file = io.open("test_output/female/other/data_female_"..sim.expIteration.."_"..sim.simIteration.."_"..ID..".csv", "w")

	for key,value in pairs(ids) do
            file:write(value ..",".. agent_table[value] .."\n")
	end

        file:close()

        file2 = io.open("test_output/female/box/box_data_"..sim.expIteration.."_"..sim.simIteration.."_"..ID..".csv", "w")
        file2:write(synced_calls .."\n")
        file2:close()

        print("Saving to: ".."test_output/female/box/box_data_"..sim.expIteration.."_"..sim.simIteration.."_"..ID..".csv\n")

        Agent.removeAgent(ID)
end
