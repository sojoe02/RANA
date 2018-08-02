local libMove = {}

torus = require "lib_torus"
collision = require "ranalib_collision"

local energy

--Move the agent if there is enough energy
function moveAgent(newPos)
  if energy.canGetHomeMotion(newPos) then
    if energy.motion() then
      if newPos.x ~= PositionX or newPos.y ~= PositionY then
        collision.updatePosition(newPos.x, newPos.y)
        return true
      end
    else
      return false
    end
  else
    return false
  end
end

function testMoveAgent(newPos)
  if energy.canGetHomeMotion(newPos) then
    if energy.motion() then
      if newPos.x ~= PositionX or newPos.y ~= PositionY then
        return true
      end
    else
      return false
    end
  else
    return false
  end
end

--This function move the robot one step closer
-- to a givin point
function libMove.testTowards(to)  
  local dir = torus.direction(
    {x = PositionX, y = PositionY},
    {x = to.x, y = to.y}
  )
  
  local new_pos = torus.getCorrectCoordinates{
    x = PositionX + dir.x, 
    y = PositionY + dir.y
  }
  
  return testMoveAgent(new_pos)
end

--So the lib can verify the agent has enough energy to move
function libMove.setEnergy(agent_energy)
  energy = agent_energy;
end

--This function move the robot one step closer
-- to a givin point
function libMove.towards(to)  
  local dir = torus.direction(
    {x = PositionX, y = PositionY},
    {x = to.x, y = to.y}
  )
  
  local new_pos = torus.getCorrectCoordinates{
    x = PositionX + dir.x, 
    y = PositionY + dir.y
  }
  
  return moveAgent(new_pos)
end

function libMove.upRight()
  local newPos = torus.getCorrectCoordinates{
    x = PositionX + 1, 
    y = PositionY - 1
  }
  
  return moveAgent(new_pos)
end

function libMove.upLeft()
  local newPos = torus.getCorrectCoordinates{
    x = PositionX - 1, 
    y = PositionY - 1
  }
  
  return moveAgent(new_pos)
end

function libMove.up()
  local newPos = torus.getCorrectCoordinates{
    x = PositionX, 
    y = PositionY - 1
  }
  
  return moveAgent(new_pos)
end

function libMove.downRight()
  local newPos = torus.getCorrectCoordinates{
    x = PositionX + 1, 
    y = PositionY + 1
  }
  
  return moveAgent(new_pos)
end

function libMove.downLeft()
  local newPos = torus.getCorrectCoordinates{
    x = PositionX - 1, 
    y = PositionY + 1
  }
  
  return moveAgent(new_pos)
end

function libMove.down()
  local newPos = torus.getCorrectCoordinates{
    x = PositionX, 
    y = PositionY + 1
  }
  
  return moveAgent(new_pos)
end

function libMove.left()
  local newPos = torus.getCorrectCoordinates{
    x = PositionX - 1, 
    y = PositionY
  }
  
  return moveAgent(new_pos)
end

function libMove.right()
  local newPos = torus.getCorrectCoordinates{
    x = PositionX + 1, 
    y = PositionY
  }
  
  return moveAgent(new_pos)
end

return libMove
 