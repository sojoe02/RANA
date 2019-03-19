local ranaLibStat = {}

-- Fetch a pseudo-random signed integer, between 'int1' and 'int2',
-- using a core centralized seed. 
-- Precision is 64 bit. 
function ranaLibStat.randomInteger(int1, int2)

	return l_getRandomInteger(int1,int2)
end

-- Fetch a  pseudo-random int, which follows a Bernouilli distribution
-- with given probability.
function ranaLibStat.bernouilliInt(prob)
	return l_getBernouilliInt(prob)
end

-- Fetch a  pseudo-random float, which follows a Gaussian distribution
-- with given mean and variance.
-- Precision is 64 bit.
function ranaLibStat.gaussianFloat(mean, var)
	return l_getGaussianFloat(mean, var)
end

-- Fetch a  pseudo-random float, which follows a Poisson distribution
-- with given mean (expected ocurrences).
-- Precision is 64 bit.
function ranaLibStat.poissonFloat(mean)
	return l_getPoissonFloat(mean)
end

-- Fetch a pseudo-random signed float between 'float1 and float2'.
-- Precision is 64 bit.
function ranaLibStat.randomFloat(float1, float2)
	return l_getRandomFloat(float1, float2)
end

-- Same as random float, except here the a deviation and mean is provided.
function ranaLibStat.randomMean(deviation, mean)

	return l_getRandomFloat(mean-deviation, mean+deviation)
end

function ranaLibStat.roundToStep(value)
	multi = 1/STEP_RESOLUTION	
	return math.floor(value*multi + 0.5)/multi
	--integer, decimal = math.modf(value*multi+0.5)
	--return integer/multi
end

return ranaLibStat
