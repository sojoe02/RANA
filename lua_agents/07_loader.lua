

-- Init of the lua frog, function called upon initilization of the LUA auton:
function initAuton(x, y, id, macroFactor, timeResolution)	

	if id == 1 then
		dofile([[/home/sojoe/qtProjects/Rana_qt/lua_agents/07_collector.lua]])
		initCollector(x,y,id, macroFactor,timeResolution)


	elseif id == 2 then
		dofile([[/home/sojoe/qtProjects/Rana_qt/lua_agents/07_scanner.lua]])
		initScanner(x,y,id, macroFactor,timeResolution)


	else
		dofile([[/home/sojoe/qtProjects/Rana_qt/lua_agents/07_vacuum.lua]])
		initVacuum(x,y,id, macroFactor,timeResolution)

	end
	
end
