local libTorus = {}

--Return the shortest distance between two points. 
function libTorus.distance(from, to)
  temp = {
    x = math.abs(to.x - from.x),
    y = math.abs(to.y - from.y)
  }
  
  if temp.x < ENV_WIDTH/2 then
    dx = temp.x
  else
    dx = ENV_WIDTH - temp.x
  end
  
  if temp.y < ENV_HEIGHT/2 then
    dy = temp.y
  else
    dy = ENV_HEIGHT - temp.y
  end
  
  return dx+dy--math.ceil(math.sqrt(dx*dx + dy*dy))
end

--Returns a valid coordinates if they a pos outsite the torus. 
function libTorus.getCorrectCoordinates(pos)  
  torus_pos = {
    x = pos.x,
    y = pos.y
  }
  
  if pos.x > ENV_WIDTH then
    torus_pos.x = 1
  elseif pos.x < 1 then
    torus_pos.x = ENV_WIDTH
  end
  
  if pos.y > ENV_HEIGHT then
    torus_pos.y = 1
  elseif pos.y < 1 then
    torus_pos.y = ENV_HEIGHT
  end
  
  return torus_pos
end

--Returns the shortest moving direction 
--as a unit vector.
function libTorus.direction(from, to)
  dx = to.x - from.x
  dy = to.y - from.y
    
  if dx > ENV_WIDTH/2 then
    dx = (dx - (ENV_WIDTH/2)) * -1
  elseif dx < -ENV_WIDTH/2 then
    dx = (dx + (ENV_WIDTH/2)) * -1
  end
  
  if dy > ENV_HEIGHT/2 then
    dy = (dy - (ENV_HEIGHT/2)) * -1
  elseif dy < -ENV_HEIGHT/2 then
    dy = (dy + (ENV_HEIGHT/2)) * -1
  end
  
  length = math.sqrt((dx*dx) + (dy*dy))
  
  direction = {
    x = round(dx/length),
    y = round(dy/length)
  }
  
  return direction
end

function round(n)
    return n % 1 >= 0.5 and math.ceil(n) or math.floor(n)
end

return libTorus
