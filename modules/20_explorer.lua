--The following global values are set via the simulation core:
-- ID -- id of the agent.
-- PositionX --	this agents x position.
-- PositionY -- this agents y position.
-- STEP_RESOLUTION 	-- resolution of steps, in the simulation core.
-- EVENT_RESOLUTION	-- resolution of event distribution.
-- StepMultiple 	-- amount of steps to skip.


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


function initializeAgent()
	C = 10000 	-- Base Capacity
	D = 0.02 	-- Ore Density
	G = 200*200 	-- Size of Grid
	I = 50		-- Communication Scope
	M = 1		-- Coorp/Comp
	N = 2		-- Number of Bases
	P = 5		-- Perception Scope	
	Q = 2		-- Cost of Motion
	T = 100000	-- Maximum Number of Cycles 
	W = 10		-- Transporter Size
	X = 4		-- Number of Explorers
	Y = 4		-- Number of Transporters
	Z = 1		-- Communication Cost
	S = X+Y		-- Memory Size of Robot
	refuel = 1000	-- Level to refuel

	
	homeCounter = 0
	segmentCounter = 0
	energyConsumption = 1000
	
	
	TargetReached = false
	endOfstep = false
	state = 0
	goto = {x = 0, y = 0}
	home = {x = PositionX, y = PositionY}
	energy_level = 1000;
	OreCoordinates = {}
	oreCounter = 0
	groupID = Shared.getNumber("index")
	Agent.changeColor(Shared.getTable("color"))
	cycleCounter = 0
	
	l_debug("Explorer agent #: " .. ID .. " has been initialized")
end

function handleEvent(sourceX, sourceY, sourceID, eventDescription, eventTable)
	if libTorus.distance({x = PositionX, y = PositionY }, {x = sourceX, y = sourceY } ) <= I then -- Control for scope	
	    if eventTable.groupID == groupID then

		if eventDescription == "requestCoordinates" then
			offloadCoordinates(sourceID)
			
		elseif eventDescription ==  "coordinatesAck" then
			--l_debug("Explorer agent #: " .. ID .. " coordinatesAck ")
			OreCoordinates = eventTable.coordinates
		
		end
	    end
	end
end

function takeStep()
	
	logData()
  
	endOfstep = false

	while( endOfstep ~= true ) do
		if state == 0 then
			checkPreConditions()		
		elseif state == 1 then
			secureMove()
		elseif state == 2 then
			lookForOre()
		elseif state == 3 then
			findNewTarget()
		elseif state == 4 then
			moveHome()
		elseif state == 5 then
			offloadCoordinates()
		end
	end
end




-- State 0
function checkPreConditions()
  
	if energy_level <= 0 then
		Map.modifyColor(PositionX, PositionY,255,0,0)
		l_removeAgent(ID)
		l_debug(".............................................................No Fuel Left")
		endOfstep = true
	elseif segmentCounter >= P then
		state = 2
	elseif TargetReached ~= true then
		state = 1
	--elseif #OreCoordinates == S then
	--	state = 4
	else
		state = 3
	end
end

-- State 1
function secureMove()
  
	if energy_level < Q *  (libTorus.distance(home, {x = PositionX, y = PositionY })  + 4*P) + P + refuel/10  then
		state = 4
	else
		libMove.towards( goto )
		segmentCounter = segmentCounter+1
		endOfstep = true
		state = 0
		energy_level = energy_level - Q
	end

	if PositionX == goto.x and PositionY == goto.y then
		TargetReached = true
		endOfstep = true
		state = 2
	end
end

-- State 2
function lookForOre()

	segmentCounter = 0
	energy_level = energy_level - P
	endOfstep = true
	state = 0

	local radialMask = Map.getRadialMask(P/2)
	for i = 1, #radialMask do
		local RGB = Map.checkColor(radialMask[i].posX,radialMask[i].posY)
		
		if RGB.R == 255 and RGB.G == 255 and RGB.B == 0 then
			Map.modifyColor(radialMask[i].posX,radialMask[i].posY,255,255,0)
			if #OreCoordinates <= S then
				table.insert(OreCoordinates, {x = radialMask[i].posX, y = radialMask[i].posY})
				oreCounter = oreCounter+1
			else
				state  = 0
			end
		end
	end


end

-- State 3
function findNewTarget()
	--TO DO implement perception of friends and calculate new destination based upon this information
	local table = Collision.radialCollisionScan(P/2)
	
	if table then
		for i = 1, #table do
			--say("\tAgent: "..table[i].id .. " at X:".. table[i].posX .. " Y:" .. table[i].posY) 
		end
	end	
		
	goto = {x = Stat.randomInteger(1,ENV_WIDTH), y = Stat.randomInteger(1,ENV_HEIGHT)}
	TargetReached = false
	endOfstep = false
	state = 1
end

-- State 4
function moveHome()
	if PositionX == home.x and PositionY == home.y then
		TargetReached = true
		state = 0
		energyConsumption = energyConsumption + ( refuel-energy_level )
		homeCounter = homeCounter + 1
		energy_level = refuel	
		endOfstep = true
		Event.emit{description="explorerRefuel",table={groupID = groupID}}
	else
		libMove.towards(home)
		endOfstep = true
		energy_level = energy_level - Q		
	end
	
end

function offloadCoordinates(senderID)
	
	if #OreCoordinates ~= 0 and secureEmit() then
		--l_debug("Explorer agent #: " .. ID .. " send " ..#OreCoordinates.. "coordinates to" ..senderID.. ".")
		Event.emit{description="offloadCoordinates", table={groupID = groupID, coordinates = OreCoordinates}, targetID = senderID}
		energy_level = energy_level - Z
	end

	state = 0
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
	cycleCounter = Shared.getNumber("cycleCounter", cycleCounter)
  
	if cycleCounter == 1 then
		file = io.open("/home/jjb/Desktop/data/Explorer_data"..ID..".csv", "w")
		file:write(cycleCounter..","..state..","..energy_level..","..#OreCoordinates..","..energyConsumption..","..homeCounter..","..oreCounter.."\n")
		file:close()
	

	elseif math.fmod(cycleCounter,100) == 0 then
		file = io.open("/home/jjb/Desktop/data/Explorer_data"..ID..".csv", "a")
		file:write(cycleCounter..","..state..","..energy_level..","..#OreCoordinates..","..energyConsumption..","..homeCounter..","..oreCounter.."\n")
		file:close()
	end
end


function cleanUp()
	l_debug("Agent #: " .. ID .. " is done\n")
end
