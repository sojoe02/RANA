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

function stateChargeOffload()
  energy.reset()
  
  --While charging and there are ore samples
  --offload them to the base
  if noOfOreSamples > 0 then
    baseNoOfOreSamples = shared.getNumber("noOfOreSamples"..baseID)
    shared.storeNumber("noOfOreSamples"..baseID, baseNoOfOreSamples + noOfOreSamples)

    noOfOreSamples = 0
  end

  state = "wait"
end

function stateHoming() 
  if home.x == PositionX and home.y == PositionY then
    state = "chargeOffload"
  --elseif #memory > 0 and noOfOreSamples < defaultValues.W then
    --nextSample = memory[#memory]
    --if move.testTowards(nextSample) then
    --  state = "collecting"
    --end
    --cycleFinished = true -- Make sure a move used a cycle
  else
    move.towards(home)
    cycleFinished = true --Make sure a move used a cycle
  end
end

function statePickup()
  map.modifyColor(nextSample.x, nextSample.y, {0, 0, 0});
  noOfOreSamples = noOfOreSamples + 1

  state = "collecting"

  cycleFinished = true  --Make sure a pickup used a cycle
end

function stateCollecting()

  if #memory > 0 and noOfOreSamples < defaultValues.W then -- This ensure if the mem is empty or the it cannor carry more it will return to home
    nextSample = memory[#memory]
    if move.towards(nextSample) then -- This ensure that the robot can get home if energi is too low
      if (nextSample.x - PositionX) < 0.5 and (nextSample.y - PositionY) < 0.5 then
        if isOre(nextSample.x, nextSample.y) then
          state = "pickup"
        end
        table.remove(memory)
      end
      cycleFinished = true -- Make sure a move used a cycle
    else
      state = "homing"
    end
  else
    state = "homing"
  end  
end

function stateWait()
  if #memory > 0 then
    state = "collecting"
  else
    cycleFinished = true -- Nothing to do just finish the cycle. 
  end
end

function isOre(x, y)

  ore_color = defaultColors["ore"];
  mapColor = map.checkColor(x, y) 

  if mapColor[1] == ore_color.r and mapColor[2] == ore_color.g and mapColor[3] and ore_color.b then
    return true
  else
    return false
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
  if #memory < defaultValues.S then
    transMemory = payload.memory
    tempMemory = {}
    --say("Transporter: " .. utility.serializeTable(transMemory))
  
    for i = 1, #transMemory do
      if isInMemory(transMemory[i]) == false or #memory == defaultValues.S then
        table.insert(memory, transMemory[i])
      else
        table.insert(tempMemory, transMemory[i])
      end
    end
    
    if msg.memory(tempMemory, sourceID) then
      skipStateProcessing = true -- Make sure that sending a message wil use one cycle
    else
      state = "homing"
    end
  else
    if msg.memory(payload.memory, sourceID) then
      skipStateProcessing = true -- Make sure that sending a message wil use one cycle
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
        --print("Transporter Agent#: " .. ID .. " has been initialized")

  
  --Retrive the default parameters
  defaultValues = shared.getTable("defaultValues")
  defaultColors = shared.getTable("defaultColors")

  --Setup the explorer
  home = {x = PositionX, y = PositionY}

  companyID = shared.getNumber("companyID")
  baseID    = shared.getNumber("baseID")
  
  agent.joinGroup(companyID)
  
  agent.changeColor(defaultColors["transporter"])
  
  msg.init{
    agentType           = "transporter",
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
  
  --This will mimic a switch case
  --Which all other reasonable languages have!
  --stupid lua
  stateAction = {
    wait          = stateWait,
    collecting    = stateCollecting,
    pickup        = statePickup,
    homing        = stateHoming,
    chargeOffload = stateChargeOffload
  }
  
  memory = {}
  
  recvMsgHandlers = {
    memory = msgMemory
  }
  
  noOfOreSamples = 0
  
  nextShout = 0
  
  state = "wait"
  cycleFinished = false
end

function takeStep()
  if skipStateProcessing == false then
    
    if nextShout < defaultValues.I then
      if #memory < defaultValues.S then
        nextShout = nextShout + 1
      end
    
      while cycleFinished == false do
        stateAction[state]()
      end
    else
      nextShout = 0
      if msg.available() == false then -- Tell explorers it missing work
        state = "homing"
      end
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
