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

x = 0.060
beta =  x + 0.030
countdown = beta

total_calls = 0
synced_calls = 0
synced = false

-- data sets
iteration = 1

function InitializeAgent()
        --l_debug("Female agent #: " .. ID .. " is being initialized")

        tbl = loadParameters("female")

        --positionX = ENV_WIDTH/2
        --positionY = ENV_HEIGHT/2
        --say("Fema "..ID.." x "..positionX.." y "..positionY)

	ids = Shared.getTable("ids")
	agent_table = Shared.getTable("agents")

        Agent.changeColor{r=255,b=255}
end

-- Init of the lua frog, function called upon initilization of the LUA auton.
function HandleEvent(event)
	--say(Utility.serializeTable(event))
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
                --say(event.ID)
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
        file = io.open("test_output/data_female_5_"..iteration.."_"..ID..".csv", "w")

	for key,value in pairs(ids) do
            file:write(value ..",".. agent_table[value] .."\n")
	end

        file:close()

        file2 = io.open("test_output/box_data_5_"..iteration.."_"..ID..".csv", "w")
        file2:write(synced_calls .."\n")
        file2:close()

        --say(Utility.serializeTable(agent_table))
        --say("Total number of calls: ".. total_calls)
        --say("Number of calls within thresshold: ".. synced_calls)

        Agent.removeAgent(ID)
        --l_debug("Female - Clean up for Agent " .. ID .. " is done")
end

--  TODO: Put function somewhere else so all agents can get to it
function loadParameters( key )

    local ftables,err = loadfile( "_parameters.data" )
    if err then
        return _,err
    end

    local tables = ftables()

    for idx = 1,#tables do

        local tolinki = {}

        for i,v in pairs( tables[idx] ) do
            --  Set so the agent knows what iteration we are doing.
            if i == "simIteration" then
                if iteration ~= nil then
                    iteration = v
                end
            end

            if i == "name" and v == key then

                if type( v ) == "table" then
                    tables[idx][i] = tables[v[1]]
                end

                if type( i ) == "table" and tables[i[1]] then
                    table.insert( tolinki,{ i,tables[i[1]] } )
                end

            end

        end

        for _,v in ipairs( tolinki ) do
            tables[idx][v[2]],tables[idx][v[1]] =  tables[idx][v[1]],nil
        end

    end

    for i = 1,#tables do
        if tables[i].name == key then
            return tables[i]
        end
    end

    return nil

end
