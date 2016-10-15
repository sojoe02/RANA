--begin_license--
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
Agent = require "ranalib_agent"
Stat = require "ranalib_statistic"
Map = require "ranalib_map"
Shared = require "ranalib_shared"
Draw = require "ranalib_draw" 

background_color = {0,0,0}
food_color = {0,255,0}

food_percentage = 0.01

population = {{speed=1},{speed=2},{speed=3}} -- various populations each with a different movement speed.
frog_amount = 40 -- number of frog agents pr. population

-- Init of the lua frog, function called upon initilization of the LUA auton.
function initializeAgent()

	StepMultiple = 1000

	say("Master Agent#: " .. ID .. " has been initialized")

	Shared.storeTable("background_color", background_color)
	Shared.storeTable("food_color", food_color)
	Shared.storeTable("population", population)

	for i = 0, ENV_WIDTH do
		for j = 0, ENV_HEIGHT do
			Map.modifyColor(i,j, background_color)
		end
	end
	
	-- Add the active agnets and setup the data collector
	local data_table = {}
	local ids = {}
	
	for key,value in pairs(population) do
		for i = 1, frog_amount do
			Shared.storeNumber("food_move_speed", value.speed)
			local ID = Agent.addAgent("11_frog.lua")
			table.insert(ids, ID)
			data_table[ID] = {call_amount = 0, movement_speed = value.speed}
		end
	end

	Shared.storeTable("agents", data_table)
	Shared.storeTable("ids", ids)

	Agent.addAgent("11_data_collector.lua",-1,-1)

	--
	--
	PositionX = -1
	PositionY = -1
	--
	--

	-- insert food
	food_total = ENV_WIDTH * ENV_HEIGHT * food_percentage

	g = 0

	for j = 1, food_total do

		local x = Stat.randomInteger(0,ENV_WIDTH)
		local y = Stat.randomInteger(0,ENV_HEIGHT)
			
		if Draw.compareColor(Map.checkColor(x,y),background_color) then
			--say("succes")

			Map.modifyColor(x,y,food_color)

		else 
			j = j - 1

		end
		g = g + 1

		if g >= ENV_WIDTH * ENV_HEIGHT* 3 then
			--break
		end
	end

end




function takeStep()

	--count number of food items
	local food_amount = 0

	for i = 0, ENV_WIDTH do
		for j = 0, ENV_HEIGHT do
			
			local current_color = Map.checkColor(i,j)

			if Draw.compareColor(Map.checkColor(i,j), food_color) then		
				food_amount = food_amount + 1

			end
			
		end
	end

	local missing_food = food_total - food_amount
	
	for j = 1, missing_food do

		local x = Stat.randomInteger(0,ENV_WIDTH)
		local y = Stat.randomInteger(0,ENV_HEIGHT)
			
		if Draw.compareColor(Map.checkColor(x,y),background_color) then
			Map.modifyColor(x,y,food_color)
		else 
			j = j - 1
		end
	end

end

