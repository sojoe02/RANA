local libEnergy = {}

torus = require "lib_torus"

local maxEnergy
local energy
local perceptionCost
local motionCost
local msgSendCost
local home

function libEnergy.current()
  return energy
end

function canGetHome(cost, newPos)
  distanceHome = torus.distance(
    {x = newPos.x, y = newPos.y},
    {x = home.x, y = home.y}
  )

  distanceHome = distanceHome + 1

  --say("Dist: " .. distanceHome)
  --say("Cost: " .. cost)
  --say("Total: " .. distanceHome * cost)
  --say("Energy1: " .. energy)
  --say("Energy2: " .. energy - cost)
  --say("")
  
  if energy - cost >= distanceHome * cost then
    return true
  else
    return false
  end   
end

function libEnergy.reset()
  energy = maxEnergy
end

function libEnergy.canGetHomeMotion(newPos)
  return canGetHome(motionCost, newPos)
end

function libEnergy.canGetHomeMsgSend()
  return canGetHome(msgSendCost, {x = PositionX, y = PositionY})
end

function libEnergy.canGetHomePerception()
  return canGetHome(perceptionCost, {x = PositionX, y = PositionY})
end

function libEnergy.init(defaultValues)
  maxEnergy       = defaultValues.energy
  energy          = defaultValues.energy
  perceptionCost  = defaultValues.perceptionCost
  motionCost      = defaultValues.motionCost
  msgSendCost     = defaultValues.msgSendCost
  home            = defaultValues.home
end

function libEnergy.motion()
  tempEnergy = energy - motionCost
  
  return updateEnergy(tempEnergy)
end

function libEnergy.msgSend()
  tempEnergy = energy - msgSendCost
  
  return updateEnergy(tempEnergy)
end

function libEnergy.perception()
  tempEnergy = energy - perceptionCost

  return updateEnergy(tempEnergy)
end

function updateEnergy(newEnergy)
  if newEnergy > 0 then
    energy = newEnergy
    return true
  else
    return false
  end
end

return libEnergy
 