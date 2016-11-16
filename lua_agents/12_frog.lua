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

-- Neighbour data
neighbour1 = {ID=0, I=0}
neighbour2 = {ID=0, I=0}

-- Call pressure at source.
P = 1

-- Import Rana lua libraries.
Event	= require "ranalib_event"
Core	= require "ranalib_core"
Stat	= require "ranalib_statistic"
Utility = require "ranalib_utility"

-- Init of the lua frog, function called upon initilization of the LUA auton.
function InitializeAgent()

	Tt = T + Stat.randomMean(e,0)
	table.insert(Olevels, Core.time()..",".. 0)
	say("Oscillator agent #: " .. ID .. " has been initialized")
end

function HandleEvent(event)

	local I = ProcessEvent{sX=event.X, sY=event.Y,pX=PositionX, pY=PositionY,table=event.table}
	
	if neighbour1.ID == 0 then
		neighbour1 = {ID=event.ID, I=I}
	end

	if neighbour2.ID == 0 then
		neighbour2 = {ID=event.ID, I=I}
	end

	-- determine neighbours

	if neighbour1.ID ~= event.ID then

		if I > neighbour1.I then

			neighbour1 = {ID=event.ID, I=I}
		end
	end

	if neighbour2.ID ~= event.ID then

		if neighbour1.ID == neighbour2.ID then

			neighbour2 = {ID=event.ID, I=I}

		elseif I > neighbour2.I and event.ID ~= neighbour1.ID then

			neighbour2 = {ID=event.ID, I=I}

		end
	end

	if neighbour1.ID == event.ID or neighbour2.ID == event.ID then

			if Tn < Tt-r then
				Tn = 0
				table.insert(Olevels, Core.time()..",".. 0.5)
				table.insert(Olevels, Core.time()..","..0)
			end

	end

end

function TakeStep()

	Tn = Tn + STEP_RESOLUTION

	if Tn >= Tt-r and peaked == false then
		local event_table = {P=P}
		Event.emit{description="Signal", table=event_table, speed=343}
		table.insert(Olevels, Core.time()..",".. 1)
		peaked = true
	end

	if Tn >= Tt then 
		Tt = T + Stat.randomMean(e, 0)
		Tn = 0
		table.insert(Olevels, Core.time()..",".. 0)
		--	say("Oscillator #"..ID.." Emitting signal at time: ".. Core.time().."[s]")
		peaked = false
	end

end

function CleanUp()
	--Write the oscillation data to a csv file.
	file = io.open("12_data"..ID..".csv", "w")

	for i,v in pairs(Olevels) do
		file:write(i..","..v.."\n")
	end
	file:close()

	say(Utility.serializeTable(neighbour1).." : " ..Utility.serializeTable(neighbour2))

	--say("Neighbours for frog # "..ID.." are : ".. neighbour1.ID .." : "..neighbour2.ID)

	l_debug("Agent #: " .. ID .. " is done\n")

end

function ProcessEvent(data)

	--distance
	local r = math.sqrt(math.pow(data.sX-data.pX,2)+math.pow(data.sY-data.pY,2)) 

	I = 1/(1+r*0.2)

	--if r > 0.2821 then

		--local I = data.table.P / r

		--local I = data.table.P / (4 * math.pi* math.pow(r,2))
		--
		

	--	return I,0
	--else 

	--	return 0.2821,0
	--end
	--
	--I = r - 10*math.log(r*r)
	--I = 1 / 1 + math.exp(-r)

	return I, .2
end

