

-- Init of the lua frog, function called upon initilization of the LUA auton:
function initAuton(x, y, id, macroFactor, timeResolution)	

		dofile([[01_agent.lua]])
        
		initAgent(x,y,id, macroFactor,timeResolution)
	
end
