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

tnum = Shared.getNumber(1)
C = Shared.getNumber(2)
D = Shared.getNumber(3)
G = Shared.getNumber(4)*Shared.getNumber(4)
I = Shared.getNumber(5)
M = Shared.getNumber(6)
N = Shared.getNumber(7)
P = Shared.getNumber(8)
Q = Shared.getNumber(9)
T = Shared.getNumber(10)
W = Shared.getNumber(11)
X = Shared.getNumber(12)
Y = Shared.getNumber(13)
Z = Shared.getNumber(14)
S = X+Y		-- Memory Size of Robot
refuel = Shared.getNumber(15)

function initializeAgent()

	l_debug("Master Agent#: " .. ID .. " has been initialized")

	for i = 1, ENV_WIDTH*ENV_HEIGHT*D do
		
		local x = Stat.randomInteger(1,ENV_WIDTH)
		local y = Stat.randomInteger(1,ENV_HEIGHT)
		local RGB = Map.checkColor(x,y)

		while RGB.R == 255 and RGB.G == 255 and RGB.B == 0 do			
			x = Stat.randomInteger(1,ENV_WIDTH)
			y = Stat.randomInteger(1,ENV_HEIGHT)
			RGB = Map.checkColor(x,y)
		end

                Map.modifyColor(x,y,{255,255,0})
	end

	l_debug("Ore's has been placed")
	
	for i=1, N do
		
		local posX =  Stat.randomInteger(1, ENV_WIDTH)
		local posY =  Stat.randomInteger(1, ENV_HEIGHT)
                Agent.addAgent("20_base.lua", posX, posY )

	end

	PositionX = -1
	PositionY = -1
	
	cycleCounter = 0
	
	
end


function takeStep()
	if cycleCounter <= T then
		cycleCounter = cycleCounter + 1
		Shared.storeNumber("cycleCounter", cycleCounter)
	else
		l_stopSimulation()
	end
	
	--Agent.removeAgent(ID)
 
end

function cleanUp()
	l_debug("Agent #: " .. ID .. " is done\n")
end

