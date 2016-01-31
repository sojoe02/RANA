----begin_license--
--
--Copyright 	2013 	Søren Vissing Jørgensen.
--			2014	Søren Vissing Jørgensen, Center for Bio-Robotics, SDU, MMMI.  
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

-- @Description:
-- This agent will oscilate between 0 and 1, with a time period. Upon peaking it will emit an event. 
-- and then rapidly fall down towards zero and restart.
--
-- Upon receiving a signal from another oscillator it will, interrupt it's current cycle, reset
-- and after a small pause it will resume a new cycle.
--
-- Data for each period will stored it a table, which upon simulation cleanUp is written
-- to the harddrive.

-- set the global variables:
myX = 0
myY = 0
ID = 0
stepPrecision = 0
eventPrecision = 0

-- data sets
Olevels = {}
dataFactor = 100
step = 0
iteration = 1

-- Oscillator values:
T = 0.500 -- time period.
e = 0.030 -- period variance with mean of 0.
r = 0.100 -- falltime.
Tt = 0 -- active period targeted time.
Tn = 0 -- active period time
y = 0.05 -- interrupt pause

peaked = false
pause = false

-- Import Rana lua libraries.
Event	= require "ranalib_event"
Core	= require "ranalib_core"
Stat	= require "ranalib_statistic"

-- Init of the lua frog, function called upon initilization of the LUA auton.
function initializeAgent(x, y, id, step_precision, event_precision)

	myX = x
	myY = y
	ID = id
	eventPrecision = event_precision
	stepPrecision = step_precision

	Tt = T + Stat.randomMean(e,0)

	l_debug("Oscillator agent #: " .. ID .. " has been initialized")
end

function takeStep()

	Tn = Tn + stepPrecision
	step = step 

	if pause == true then
		if Tn > y then
			table.insert(Olevels, Core.time()..","..0)
			pause = false
		end
	end

	if Tn >= Tt-r and peaked == false then
		Event.emit{description="Signal"}
		table.insert(Olevels, Core.time()..",".. 1)
		peaked = true

	end

	if Tn >= Tt then 
		Tt = T + Stat.randomMean(e, 0)
		Tn = 0
		table.insert(Olevels, Core.time()..",".. 0)
		l_print("Interrupt Oscillator "..ID.." Emitting signal at time: ".. Core.time().."[s]")
		peaked = false
	end
end

function handleEvent(sourceX, sourceY, sourceID, eventDescription, eventTable)
	
	if peaked == false then
		-- write data to the Olevel table:
		table.insert(Olevels, Core.time()..","..Tn/Tt)
		table.insert(Olevels, Core.time()+eventPrecision..","..0)
		--calculate new period
		Tt = T + Stat.randomMean(e, 0) + y
		Tn = 0
		pause = true
	end


end

function synchronizePosition()
	return myX, myY
end

function cleanUp()

	--Write the oscillation data to a csv file.
	file = io.open("03_data"..ID..".csv", "w")
	for i,v in pairs(Olevels) do
		file:write(i..","..v.."\n")
	end
	file:close()

	l_debug("Agent #: " .. ID .. " is done\n")
end

