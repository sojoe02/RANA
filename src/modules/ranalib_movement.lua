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

function ranaLibMovement.setDirection(angle)

end
	

	

return ranaLibMovement
