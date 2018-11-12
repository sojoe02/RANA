local ranaLibCore = {}

-- Halt the simulation. This will emit a stop signal to Ranas core.
-- the cleanUp function will be called for all agents. Event processing will be halted, 
-- if the function is run during takeStep() all agents will be able to take their steps prior
-- to halting the simulation.
function ranaLibCore.stopSimulation()
	
	l_stopSimulation()
end

-- Get the time of off the simulation core in seconds, with 64 bit precision.
function ranaLibCore.time()

	return l_currentTimeS()		
end

return ranaLibCore
