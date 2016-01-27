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

-- Fetch a pseudo-random signed integer, between 'int1' and 'int2',
-- using a core centralized seed. 
-- Precision is 64 bit. 
function ranaLibCore.randomInteger(int1, int2)

	return l_getRandomInteger(int1,int2)
end

-- Fetch a pseudo-random signed float between 'float1 and float2'.
-- Precision is 64 bit.
function ranaLibCore.randomFloat(float1, float2)

	return l_getRandomFloat(float1, float2)
end

-- Same as random float, except here the a deviation and mean is provided.
function ranaLibCore.randomMean(deviation, mean)

	return l_getRandomFloat(mean-deviation, mean+deviation)
end

return ranaLibCore
