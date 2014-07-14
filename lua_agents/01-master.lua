

-- Init of the lua frog, function called upon initilization of the LUA auton:
function initAuton(x, y, id, macroFactor, timeResolution)

	if id == 1 then
		dofile("/home/sojoe/Documents/MSC/SimCmake/test/07_collector.lua")
		initCollector(x,y,id, macroFactor,timeResolution)
	else
		dofile("/home/sojoe/Documents/MSC/SimCmake/test/07_asynchronized.lua")
		initASynchronizer(x, y, id, macroFactor, timeResolution)	

	end

end

