local ranaLibMath = {}

function ranaLibMath.calcDistance(data)
	
	return math.sqrt(math.pow(data.x1-data.x2,2)+ math.pow(data.y1-data.y2,2))

end

function ranaLibMath.calcAngle(data)


		Angle = math.atan2(data.y1-data.y2, data.x1-data.x2)*57.2958

		if Angle < 0 then
			Angle = Angle + 360

		end
		
		return(Angle)
	

end

return ranaLibMath
