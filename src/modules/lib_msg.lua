local libMsg = {}

event = require "ranalib_event"
energy = require "lib_energy"
torus = require "lib_torus"

local agentType
local baseID
local companyID
local communicationScope

function libMsg.init(defaultValues)
  agentType           = defaultValues.agentType
  baseID              = defaultValues.baseID
  companyID           = defaultValues.companyID
  communicationScope  = defaultValues.communicationScope
end

function sendMsg(msgType, msgData, to, company)
  if energy.canGetHomeMsgSend() then
    if energy.msgSend() then
      event.emit{description = msgType, table = msgData, targetID = to}
      return true
    else
      return false
    end
  else
    return false
  end
end

function libMsg.memory(memory, to)
  local msgData = {
    agentType = agentType,
    companyID = companyID,
    payload = {memory = memory}
  }

  return sendMsg("memory", msgData, to, companyID)  
end

function libMsg.available()
  local msgData = {
    agentType = agentType,
    companyID = companyID
  }

  return sendMsg("available", msgData, 0, companyID)  
end

function libMsg.isValid(sourceX, sourceY)
  distance = torus.distance(
    {x = sourceX, y = sourceY},
    {x = PositionX, y = PositionY}
  )
  
  if distance <= communicationScope then
    return true
  else
    return false
  end  
end

return libMsg
 