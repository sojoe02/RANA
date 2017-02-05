local ranaLibMath = {}

function ranaLibMath.calcDistance(data)
	
	return math.sqrt(math.pow(data.x1-data.x2,2)+ math.pow(data.y1-data.y2,2))

end

function ranaLibMath.calcAngle(data)


		local angle = math.atan2(data.y1-data.y2, data.x1-data.x2)*57.2958

		if angle < 0 then
			angle = Angle + 360

		end
		
		return(angle)
	

end

return ranaLibMath
