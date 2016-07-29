local ranaLibMovement = {}

-- Move to a postion on the map if the right option arguments are given.
-- Example: to{x=100, x=100, speed=4} Moves to a position 100,100 on the map 
-- with a speed of 5 meters pr second
function ranaLibMovement.to(options)

	local X = options.x or posX
	local Y = options.y or posY
	local collision = options.collision or false
	
	ranaLibMovement.setSpeed(options.speed)

	Moving = true
	DestinationX = X
	DestinationY = Y

end

--Moves to a valid random position on the map, movementspeed is optional.
function ranaLibMovement.toRandom(speed)

	local speed = speed or nil

	ranaLibMovement.to{x=l_getRandomInteger(0,ENV_WIDTH), y=l_getRandomInteger(1,ENV_HEIGHT), speed=speed}

end

-- Sets movement speed of the agent
-- @Param speed the speed of the agent in meters pr. second. has to have a value above 0
function ranaLibMovement.setSpeed(speed)

	if type(speed) == "number" and speed > 0 then
		MovementSpeed = speed	
	end

end

return ranaLibMovement
