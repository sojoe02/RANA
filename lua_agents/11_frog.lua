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
Move = require "ranalib_movement"
Map = require "ranalib_map"
Shared = require "ranalib_shared"
Agent = require "ranalib_agent"

--load the behaviour modules
Forage = require "11_forage_module"
Oscillate = require "11_oscillator_module"

Food = 0

state_foraging 		= "state_for"
state_oscillating 	= "state_osc"
state = state_foraging

-- Init of the lua frog, function called upon initilization of the LUA auton.
function InitializeAgent()

	local background_color = Shared.getTable("background_color")
	local food_color 	= Shared.getTable("food_color")
	--local forage_amount = Shared.getNumber("forage_amount")
	local food_move_speed = Shared.getNumber("food_move_speed")

	--say(food_move_speed)
	
	Forage.configure{search_radius=5, 
					search_move_radius=20, 
					move_speed=food_move_speed, 
					background_color=background_color, 
					food_color=food_color,
					forage_amount=5}

	Oscillate.configure()
	Agent.changeColor{r=0, g=100, b=200}
end


function takeStep()

	--check state
	if state == state_foraging then
		
		Forage.TakeStep()
		
		if Food >= 20 then
			state = state_oscillating
			Agent.changeColor{r=200, g= 0, b=0}
		end

	elseif state == state_oscillating then
		
		Oscillate.TakeStep()
		
		if Food <= 0 then
			state = state_foraging
			Agent.changeColor{r=0, g=100, b=200}
		end
	end

end

function cleanUp()
	l_debug("Prey #: " .. ID .. " has " .. Food .. " Food" )
end

