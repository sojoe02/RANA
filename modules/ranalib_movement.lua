local ranaLibMovement = {}

local degrees = 57.2958

-- Move to a postion on the map if the right option arguments are given.
-- Example: to{x=100, x=100, speed=4} Moves to a position 100,100 on the map 
-- with a speed of 5 meters pr second
function ranaLibMovement.to(options)


	local xx = options.x or posX
	local yy = options.y or posY
	local sspeed = options.speed or Speed
	local matchAngle = options.matchAngle or true

	ranaLibMovement.setSpeed(sspeed)

	--say(sspeed ..","..Speed)


	--if xx > ENV_WIDTH then xx = ENV_WIDTH end
	--if xx < 0 then xx = 0 end
	--if yy > ENV_HEIGHT then yy = ENV_HEIGHT end
	--if yy < 0 then yy = 0 end


	Moving = true

	DestinationX = xx
	DestinationY = yy

	--if matchAngle == true then

		Angle = math.atan2(DestinationY-PositionY, DestinationX-PositionX)*57.2958
		if Angle < 0 then
			Angle =  360+Angle
		elseif Angle > 360 then
			Angle = 360-Angle


		end
		--say(Angle)
		
	--end

end

function ranaLibMovement.byAngle(angle_var)


	local radius = 0
	local radiusWidth = ENV_WIDTH+PositionX
	local radiusHeight = ENV_HEIGHT+PositionY

	if radiusWidth > radiusHeight then 
		radius = radiusWidth
	else
		radius = radiusHeight
	end

	local angle = angle_var / degrees

	local x =PositionX + radius * math.cos(angle)  
	local y =PositionY + radius * math.sin(angle) 

	--say(y)
--	say(x)

	ranaLibMovement.to{x=x,y=y}


end

--Moves to a valid random position on the map, movementspeed is optional.
function ranaLibMovement.toRandom(speed)

	local speed = speed or Speed

	ranaLibMovement.to{x=l_getRandomInteger(0,ENV_WIDTH), y=l_getRandomInteger(0,ENV_HEIGHT), speed=speed}

end

-- Sets movement speed of the agent
-- @Param speed the speed of the agent in meters pr. second. has to have a value above 0
function ranaLibMovement.setSpeed(speed)

	if type(speed) == "number" and speed > 0 then
		Speed = speed
	end

end

-- Sets the X and Y velocity components of the agent
-- @Param velocity{x, y} in m/s. Can be negative
function ranaLibMovement.setVelocity(velocity)

	local vx = velocity.x
	local vy = velocity.y

	local deltaX = -10
	local deltaY = -10

	local deltaX_2 = 0
	local deltaY_2 = 0

	-- Get the movement angle, in radians
	local angle = math.atan2(vy, vx)

	-- Get the absolute speed
	local speed = math.sqrt(math.pow(vx, 2) + math.pow(vy, 2))

	-- Get the X and Y distances from the agent to the map border, in the
	-- direction of its movement.
	if vx > 0 then
		deltaX = ENV_WIDTH - PositionX
	elseif vx < 0 then
		deltaX = PositionX
	end

	if vy > 0 then
		deltaY = ENV_HEIGHT - PositionY
	elseif vy < 0 then
		deltaY = PositionY
	end

	-- Get the X and Y distances from the agent to the crossing between the
	-- projection of its movement and the borders of the map
	deltaX_2 = math.abs(deltaY / math.tan(angle))
	deltaY_2 = math.abs(deltaX * math.tan(angle))

	-- Get the destination, based on the projection of the velocity over
	-- both borders in the map, and the direction of the movement.
	-- For X component.
	if deltaY_2 <= deltaX_2 then
		if vx < 0 then
			DestinationX = 0
		elseif vx > 0 then
			DestinationX = ENV_WIDTH
		else
			DestinationX = PositionX
		end
	else
		if vx < 0 then
			DestinationX = PositionX - deltaX_2
		elseif vx > 0 then
			DestinationX = PositionX + deltaX_2
		else
			DestinationX = PositionX
		end
	end
	-- For Y component.
	if deltaY_2 >= deltaX_2 then
		if vy < 0 then
			DestinationY = 0
		elseif vy > 0 then
			DestinationY = ENV_HEIGHT
		else
			DestinationY = PositionY
		end
	else
		if vy < 0 then
			DestinationY = PositionY - deltaY_2
		elseif vy > 0 then
			DestinationY = PositionY + deltaY_2
		else
			DestinationY = PositionY
		end
	end


	-- if deltaX_2 > deltaY_2 then
	-- 	DestinationX = ENV_WIDTH
	-- 	DestinationY = PositionY + deltaY_2
	-- elseif deltaX_2 < deltaY_2 then
	-- 	DestinationX = PositionX + deltaX_2
	-- 	DestinationY = ENV_HEIGHT
	-- else
	-- 	DestinationX = ENV_WIDTH
	-- 	DestinationY = ENV_HEIGHT
	-- end

	ranaLibMovement.setSpeed(speed)
	Moving = true

end
	

	

return ranaLibMovement
