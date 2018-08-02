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
Core	= require "ranalib_core"


-- Init of the lua frog, function called upon initilization of the LUA auton.
function initializeAgent()

        l_debug("Transporter #: " .. ID .. " is being initialized")

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

	
	cycleCounter = 0
	TargetReached = true
	endOfstep = false
	state = 0
	goto = {x = 0, y = 0}
	home = {x = PositionX, y = PositionY}
	energy_level = 1000
	energyConsumption = 1000
	homeCounter = 0
	OreCoordinates = {}
	OreLoaded = 0
	oreCounter = 0
	groupID = Shared.getNumber("index")
	Agent.changeColor(Shared.getTable("color"))
	l_debug("Transporter agent #: " .. ID .. " has been initialized")
end

function handleEvent(sourceX, sourceY, sourceID, eventDescription, eventTable)
	if libTorus.distance({x = PositionX, y = PositionY }, {x = sourceX, y = sourceY } ) <= I then -- Control for perceptionscope
	    if eventTable.groupID == groupID then
			if eventDescription == "offloadCoordinates" then
				recieveCoordinates(senderID, eventTable.coordinates)
			end
	    end
	end
end

function takeStep()
	logData() 
	endOfstep = false

	while( endOfstep ~= true ) do
		endOfstep = true
		if state == 0 then
			checkPreConditions()		
		elseif state == 1 then
			secureMove()
		elseif state == 2 then
			fetchOre()
		elseif state == 3 then
			findNewTarget()
		elseif state == 4 then
			moveHome()
		elseif state == 5 then
			getCoordinates()
		end
	end
end

-- State 0
function checkPreConditions()
	if energy_level <= 0 then
		l_removeAgent(ID)
		l_debug("................................... Transporter No Fuel Left")
		endOfstep = true
	elseif OreLoaded >= W then
		state = 4
	elseif TargetReached ~= true then
		state = 1
	else
		state = 3
	end
end

-- State 1
function secureMove()
	if energy_level <= ( Q *  libTorus.distance(home, {x = PositionX, y = PositionY } )) + refuel/10 then
	      state = 4
	else
		libMove.towards( goto )
		endOfstep = true
		state = 0
		energy_level = energy_level - Q
	end

	if PositionX == goto.x and PositionY == goto.y then
		TargetReached = true
		state = 2
	end
end

-- State 2
function fetchOre()
	local RGB = Map.checkColor(PositionX,PositionY)

        print("state 2")

	if RGB.R == 255 and RGB.G == 255 and RGB.B == 0 then
 		Map.modifyColor(PositionX,PositionY,0,0,0)
		--endOfstep = true
		OreLoaded = OreLoaded + 1
		oreCounter = oreCounter +1
	end
	state = 3
end

-- State 3
function findNewTarget()
	state = 0
	endOfStep = true
	
	getCoordinates()
	
	if #OreCoordinates ~= 0 then
		for i = 1, #OreCoordinates do
			local requiredEnergy = Q * (libTorus.distance({x = PositionX, y = PositionY }, OreCoordinates[i]) + libTorus.distance(OreCoordinates[i], home))

			if 	requiredEnergy <= energy_level then 
 				goto = table.remove(OreCoordinates, i)
                                --l_debug("New Ore Target Set")
				TargetReached = false
				state = 0
				endOfStep = true			
				break  
			end	
		end
		
		if TargetReached ~= false then
			state = 4
			OreCoordinates = {} -- If nothing is poosible - go home and empty list of coordinates
		end
	else
		state = 4
                --l_debug("Transporter "..ID.. " Out of Targets")
	end
	
end

-- State 4
function moveHome()
	if PositionX == home.x and PositionY == home.y then
		state = 0
		energyConsumption = energyConsumption + ( refuel-energy_level )
		homeCounter = homeCounter + 1
		energy_level = refuel
		offLoadOre()
	else
		libMove.towards(home)
		endOfstep = true
		energy_level = energy_level - Q
		state = 4
	end
end

-- State 5
function getCoordinates()
	if PositionX ~= home.x and PositionY ~= home.y then
	    requestCoordinates()
	elseif l_getMersenneInteger(1,10) == 1 then
	    requestCoordinates()
	end
	
end


function requestCoordinates()
	if (W - OreLoaded) > 0 and secureEmit() then
		Event.emit{description="requestCoordinates",table={groupID = groupID} }
		energy_level = energy_level - Z
	end
end


function recieveCoordinates(senderID, coordinates)
	local debugCounter = 0
	while #OreCoordinates <= S and #coordinates ~= 0 do	
		debugCounter = debugCounter + 1	
		table.insert(OreCoordinates, table.remove(coordinates) )
	end	

		--l_debug("Transporter " ..ID.. " receaved " ..debugCounter.. " Coordinates. Has " .. #OreCoordinates.. " In stock" )
 
	Event.emit{description="coordinatesAck",table= {groupID = groupID, coordinates = coordinates}, targetID = senderID}
	energy_level = energy_level - Z
	
end


function offLoadOre()
	if OreLoaded ~= 0 then
		Event.emit{ description="offloadOre",table = { groupID = groupID, ores = OreLoaded } }
		-- No Communication needed --energy_level = energy_level - Z
		OreLoaded  = 0
	end
end


function secureEmit()
	if energy_level >= ( Q *  libTorus.distance(home, {x = PositionX, y = PositionY } ) ) + Z  then
		return true
	else
		--l_debug("Cannot Send")
		return false
	end
end


function logData() 
        cycleCounterNum = Shared.getNumber("cycleCounter")
  
        if cycleCounterNum == 1 then
                file = io.open("/home/theis/Dropbox/mas/data/Transporter_data"..ID..".csv", "w")
                file:write(cycleCounterNum..","..state..","..energy_level..","..#OreCoordinates..","..energyConsumption..","..homeCounter..","..oreCounter.."\n")
		file:close()
        elseif math.fmod(cycleCounterNum,100) == 0 then
                file = io.open("/home/theis/Dropbox/mas/data/Transporter_data"..ID..".csv", "a")
                file:write(cycleCounterNum..","..state..","..energy_level..","..#OreCoordinates..","..energyConsumption..","..homeCounter..","..oreCounter.."\n")
		file:close()
	end
end


function cleanUp()
	l_debug("Agent #: " .. ID .. " is done\n")
end
