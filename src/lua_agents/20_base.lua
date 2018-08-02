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
libMove = require "lib_move"
libTorus = require "lib_torus"
Stat = require "ranalib_statistic"
Collision = require "ranalib_collision"
Utility = require "ranalib_utility"
Map = require "ranalib_map"
Agent = require "ranalib_agent"
Shared = require "ranalib_shared"
Utility = require "ranalib_utility"
Core	= require "ranalib_core"


-- Init of the lua frog, function called upon initilization of the LUA auton.
function initializeAgent()

        l_debug("Base #: " .. ID .. " is being initialized")

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


        print(tnum.." "..C.." "..D.." "..G.." "..I.." "..M.." "..N.." "..P.." "..Q.." "..T.." "..W.." "..X.." "..Y.." "..Z.." "..S.." "..refuel)

	oreCounter  = 0
	cycleCounter = 0
	tRefuelCounter = 0
	eRefuelCounter = 0
	
	home = {x = PositionX, y = PositionY}
	OreCoordinates = {}

	if M == 0 then
	    groupID = ID
	elseif M == 1 then
	    groupID = 1
	end

	local RGB = {r = Stat.randomInteger(100,255), g = Stat.randomInteger(100,255), b = Stat.randomInteger(100,255)}
	Agent.changeColor(RGB)
	Shared.storeTable("color", RGB )
	Shared.storeNumber("index", groupID)

	for j=1, X do
            local explorerID = Agent.addAgent("20_explorer.lua",PositionX, PositionY)
	end

	for j=1, Y do
            local explorerID = Agent.addAgent("20_transporter.lua",PositionX, PositionY)
	end
	
	l_debug("Base #: " .. ID .. " has been initialized")
	
end

function handleEvent(sourceX, sourceY, sourceID, eventDescription, eventTable)
	
	if libTorus.distance(home, {x = sourceX, y = sourceY }) <= I then -- Control for perceptionscope
	    if eventTable.groupID == groupID then
		
			if eventDescription == "offloadOre" then
				tRefuelCounter = tRefuelCounter + refuel
				local number = eventTable.ores
				if oreCounter + number <= C then		
					oreCounter  = oreCounter + number
					l_debug("Recieved: " .. number .. " Ores")
				else
					oreCounter = C
				end
			elseif eventDescription == "explorerRefuel" then
			eRefuelCounter = eRefuelCounter + refuel
			end
	    end
	end
end

function takeStep()
	logData()
end

function logData()
        cycleCounterNum = Shared.getNumber("cycleCounter")

        if cycleCounterNum == 1 then
                file = io.open("/home/theis/Dropbox/mas/data/Base_data"..ID..".csv", "w")
                file:write(cycleCounterNum..","..eRefuelCounter..","..tRefuelCounter..","..X..","..Y..","..oreCounter.."\n")
		file:close()
        elseif math.fmod(cycleCounterNum,100) == 0 then
                file = io.open("/home/theis/Dropbox/mas/data/Base_data"..ID..".csv", "a")
                file:write(cycleCounterNum..","..eRefuelCounter..","..tRefuelCounter..","..X..","..Y..","..oreCounter.."\n")
		file:close()
	end
end


function cleanUp()
	l_debug("Base #: " .. ID .. " is done\n")
end
