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

-- Import valid Rana lua libraries.
Event = require "ranalib_event"
Shared = require "ranalib_shared"	

-- Init of the lua frog, function called upon initilization of the LUA auton.
function initializeAgent()

	l_debug("Agent #: " .. ID .. " has been initialized")

end

function handleEvent(sourceX, sourceY, sourceID, eventDescription, eventTable)

	if eventDescription == "table" then
		local table = Shared.getTable(eventTable.index)
		say("Agent:"..ID.." got a table with values:"..table[1]..","..table[2]..","..table[3]..","..table[4])
	elseif eventDescription == "number" then
		local number = Shared.getNumber(eventTable.index)
		say("Agent:"..ID.." got a number which is "..number)
	elseif eventDescription == "string" then
		local string = Shared.getString(eventTable.index)
		say("Agent:"..ID.." got a string that says : "..string)
	end

end

function takeStep()

	local Rand = l_getMersenneInteger(1,1000)
	local index

	if Rand == 1 and ID==1 then
	
		index = "table"
		say("Agent:"..ID.." is submitting a table to shared ")
		Shared.storeTable(index, {1,2,4,5,6})
	 	Event.emit{description="table",table={index=index}}

	elseif Rand == 2 and ID==1 then
		
		index = "number"
		local number = l_getMersenneFloat(0,10)
		say("Agent:"..ID.." is submitting number: "..number.." to shared ") 
		Shared.storeNumber(index, number)
		Event.emit{description="number",table={index=index}}

	elseif Rand == 3 and ID == 1 then

		index = "string"
		say("Agent:"..ID.." is submitting a string to shared ")
		Shared.storeString(index, "Agent "..ID.." stored this string")
		Event.emit{description="string",table="{index=\""..index.."\"}"}
		
	end

end

