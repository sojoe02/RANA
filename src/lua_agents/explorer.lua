agent   = require "ranalib_agent"
shared  = require "ranalib_shared"	
move    = require "lib_move"
energy  = require "lib_energy"
stat    = require "ranalib_statistic"
draw    = require "ranalib_draw"
map     = require "ranalib_map"
torus   = require "lib_torus"
msg     = require "lib_msg"
utility = require "ranalib_utility"

--[[
******************************************************************************
State proccessing functions start
******************************************************************************
--]]
function stateWait()
  if #memory < defaultValues.S then
    state = "exploring"
    tempX = coverageX - 1;
    tempY = coverageY;
    nextPerception = defaultValues.P
    --tempX = PositionX
    --tempY = PositionY
  else
    cycleFinished = true --nothing to do so run a cycle
  end  
end

function stateHoming() 
  if home.x == PositionX and home.y == PositionY then
    state = "charge"
  --elseif #memory < defaultValues.S then
    --if move.testTowards{x = tempX, y = tempY} then
    --  state = "wait"
    --end
    --cycleFinished = true -- Make sure a move used a cycle
  else
    move.towards(home)
    cycleFinished = true -- Make sure a move used one cycle
  end
  
  exploring = false
end

function stateCharge()
  energy.reset() 
  state = "wait"
end

function statePerception()
  if energy.canGetHomePerception() then
      if energy.perception() then    
        findOreSamples()
        if #memory == defaultValues.S then -- if memory is full go home to find a transporter
          state = "homing"
        else
          state = "exploring"
        end
        
        cycleFinished = true -- Make sure a perception used a cycle
      end
  else
    state = "homing"
  end
end

function stateExploring()
  if tempX == PositionX and tempY == PositionY then
    coveragePlanning()
    if PositionX ~= home.x and PositionY ~= home.y then
      exploring = true
    end
    --tempX = stat.randomInteger(1, ENV_WIDTH)
    --tempY = stat.randomInteger(1, ENV_HEIGHT)
  end
  
  if move.towards{x = tempX, y = tempY} then
    if exploring then
      nextPerception = nextPerception + 1
      
      if nextPerception > defaultValues.P/2 then
        nextPerception = 0
        state = "perception"
      end
    end
    cycleFinished = true -- make sure a move used a cycle 
  else
    state = "homing"
  end
end

function coveragePlanning()
  if PositionX == home.x then    
    coverageY = coverageY + defaultValues.P/2*defaultValues.X
    
    if coverageY > ENV_HEIGHT then
      coverageY = coverageY - ENV_HEIGHT
    end
  end
  
  coverageX = coverageX + 1
  if coverageX == ENV_WIDTH - 1 then
    coverageX = 2
  end
  --pos = torus.getCorrectCoordinates{x = coverageX, y = coverageY}
  
  --coverageX = pos.x
  --coveragey = pos.y
  
  tempX = coverageX
  tempY = coverageY
end

function findOreSamples()
  perceptionCoords = map.getRadialMask(defaultValues.P/2)
  
  ore_color = defaultColors["ore"];
  
  for i = 1, #perceptionCoords do 
    correctedPos = torus.getCorrectCoordinates{
      x = perceptionCoords[i].posX,
      y = perceptionCoords[i].posY
    }
    
    --map.modifyColor(correctedPos.x, correctedPos.y, 255, 255, 255);
    
    mapColor = map.checkColor(correctedPos.x, correctedPos.y) 

    if mapColor[1] == ore_color.r and mapColor[2] == ore_color.g and mapColor[3] and ore_color.b then
      if #memory < defaultValues.S then
        if isInMemory({x = correctedPos.x, y = correctedPos.y}) == false then
          table.insert(memory, {x = correctedPos.x, y = correctedPos.y})
        end
      end
    end          
  end
end

--[[
******************************************************************************
State proccessing functions end
******************************************************************************
--]]

--[[
******************************************************************************
Message function start
******************************************************************************
--]]

function msgMemory(sourceID, payload) 
  memory = payload.memory
  waitingForReponse = false
  --say("Explorer: " .. utility.serializeTable(payload.memory))
end

function msgAvailable(sourceID, payload)
  if #memory > 0 and waitingForReponse == false then
    if msg.memory(memory, sourceID) then
      skipStateProcessing = true -- Make sure that sending a message wil use one cycle
      waitingForReponse = true
    else
      state = "homing"
    end
  end
end

--[[
******************************************************************************
Message function end
******************************************************************************
--]]

--[[
******************************************************************************
RANA function start
******************************************************************************
--]]
-- Initialization of the agent.
function initializeAgent()
        --say("Explorer Agent#: " .. ID .. " has been initialized")
  
  --Retrive the default parameters
  defaultValues = shared.getTable("defaultValues")
  defaultColors = shared.getTable("defaultColors")

  --Setup the explorer
  home = {x = PositionX, y = PositionY}
  
  companyID = shared.getNumber("companyID")
  baseID    = shared.getNumber("baseID")
  
  agent.joinGroup(groupID)
  
  agent.changeColor(defaultColors["explorer"])
  
  msg.init{
    agentType           = "explorer",
    baseID              = baseID,
    companyID           = companyID,
    communicationScope  = defaultValues.I
  }
  
  energy.init{
    energy          = defaultValues.E,
    perceptionCost  = defaultValues.P,
    motionCost      = defaultValues.Q,
    msgSendCost     = defaultValues.A,
    home            = home
  }
  
  move.setEnergy(energy);
  
  stateAction = {
    exploring   = stateExploring,
    perception  = statePerception,
    homing      = stateHoming,
    charge      = stateCharge,
    wait        = stateWait
  }
  
  memory = {}
  
  tempX = PositionX
  tempY = PositionY
  coverageX = PositionX
  coverageY = PositionY + defaultValues.P/2 * (ID - baseID - 1) -- Set the start coverage offset
  coverageHeight = 0
  
  exploring = false
  nextPerception = 0
  state = "exploring"
  
  cycleFinished = false
  waitingForReponse = false
  skipStateProcessing = false
  
  recvMsgHandlers = {
    available = msgAvailable,
    memory    = msgMemory
  }
end

function takeStep()
  if skipStateProcessing == false then
    while cycleFinished == false do
      stateAction[state]()
    end
  end
    
  skipStateProcessing = false
  cycleFinished = false
end

function handleEvent(sourceX, sourceY, sourceID, eventDescription, eventTable)  
  --Check is this is a message is should be able to "see" otherwise ignore it
  if msg.isValid(sourceX, sourceY) and companyID == eventTable.companyID then
    if recvMsgHandlers[eventDescription] then
      recvMsgHandlers[eventDescription](sourceID, eventTable.payload)
    end
  end
end

--[[
******************************************************************************
RANA function end
******************************************************************************
--]]

function isInMemory(coord)
  found = false
  for j = 1, #memory do
    if coord.x == memory[j].x and coord.y == memory[j].y then
      found = true
    end
  end
  
  return found
end
