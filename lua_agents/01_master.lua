

-- Init of the lua frog, function called upon initilization of the LUA auton:
function initAuton(x, y, id, macroFactor, timeResolution)	

	--dofile([[C:/Users/Sojoe/Documents/qt/rana/RANA_QT/lua_agents/01_agent.lua]]) 
	dofile([[/home/sojoe/qtProjects/Rana_qt/lua_agents/01_agent.lua]])

	initAgent(x,y,id, macroFactor,timeResolution)
	
end
