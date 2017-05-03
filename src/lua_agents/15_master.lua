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
-- Speed 		-- Movement speed of the agent in meters pr. second.
-- Moving 		-- Denotes wether this agent is moving (default = false).
-- GridMove 		-- Is collision detection active (default = false).
-- ------------------------------------

-- Import Rana lua libraries.
Agent   = require "ranalib_agent"
Shared  = require "ranalib_shared"
Stat    = require "ranalib_statistic"
Sim     = require "ranalib_simconfig"
Utility = require "ranalib_utility"

--  Data
sim = {}

-- Init of the lua frog, function called upon initilization of the LUA auton.
function InitializeAgent()

    -- Add the data collector agent.
    PositionX = -1
    PositionY = -1

    agent_table = {}
    ids = {}

    sim = Utility.loadTable("master", "_parameters.data")

    initiateGreenfield()

    Shared.storeTable("agents", agent_table)
    Shared.storeTable("ids", ids)

    local ID = Agent.addAgent("15_female.lua", 100, 100)
end

function initiateGreenfield()
    -- Load up the greenfield oscillator agents.
    local ID = Agent.addAgent("15_greenfield_osc.lua", 100-sim.v2, 100)
    table.insert(ids, ID)
    agent_table[ID]=0

    local ID = Agent.addAgent("15_greenfield_osc.lua", 100+sim.v2, 100)
    table.insert(ids, ID)
    agent_table[ID]=0
end

function initiateFreerunning()
    -- Load up the free oscillator agent
    local ID = Agent.addAgent("15_freerunning_osc.lua", 100, 110)

    table.insert(ids,ID)
    agent_table[ID]=0
end

function takeStep()
end

function cleanUp()
    Agent.removeAgent(ID)
end

