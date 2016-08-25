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

-- data sets
Olevels = {}
iteration = 1

-- Oscillator values:
T = 0.500 -- Time period.
e = 0.030 -- Period variance with mean of 0.
r = 0.100 -- falltime.
Tt = 0 -- active period targeted time. Peak is then equal to Tt-r
Tn = 0 -- active period time
peaked = false -- boolean helping with numeric resolution issues.

-- Import Rana lua libraries.
Event	= require "ranalib_event"
Core	= require "ranalib_core"
Stat	= require "ranalib_statistic"

-- Init of the lua frog, function called upon initilization of the LUA auton.
function initializeAgent()

	Tt = T + Stat.randomMean(e,0)
	table.insert(Olevels, Core.time()..",".. 0)
	say("Oscillator agent #: " .. ID .. " has been initialized")
end

function takeStep()

	Tn = Tn + STEP_RESOLUTION

	if Tn >= Tt-r and peaked == false then
		Event.emit{description="Signal"}
		table.insert(Olevels, Core.time()..",".. 1)
		peaked = true

	end

	for i = 1, 10000 do
		local s = math.sqrt(i*i)
	end

	if Tn >= Tt then 
		Tt = T + Stat.randomMean(e, 0)
		Tn = 0
		table.insert(Olevels, Core.time()..",".. 0)
--		say("Oscillator #"..ID.." Emitting signal at time: ".. Core.time().."[s]")
		peaked = false
	end

end

function cleanUp()
	if ID == 1 then
	--Write the oscillation data to a csv file.
		file = io.open("02_data"..ID..".csv", "w")
		for i,v in pairs(Olevels) do
			file:write(i..","..v.."\n")
		end
		file:close()
	end
	l_debug("Agent #: " .. ID .. " is done\n")
end

