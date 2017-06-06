    --  Variables used to set the simulation
simname = "_simconfig.data"
parname = "_parameters.data"

currentSimulation = 1           --  Current simulation
totalIterations = 5             --  The total number of simulations
currentExperiments = 1          --  Current experiment
numOfExperiments = 1            --  How many simulations with the same parameters do we want


--  Import Rana lua libraries
Utility = require "ranalib_utility"

--  Master table to be set with parameters
configTbl =
{
    {
        name="simulation",
        currentSimIteration=1,  --Which simulation are we doing.
        expIteration=1          --How many times have we run the same experiment
    },
    {
        name="master",
        v1_f = 0.0,
        v1_t = 1.01,
        v2_f = 0.0,
        v2_t = 100.0,
        v1_ss = 0.01,
        v2_ss = 0.2,
        numVar=2
    }
}

function _getInitialCurrentSimulation()

    if currentSimulation == nil then
        return 1
    end

    return currentSimulation

end

function _getTotalNumberSimulation()

    if totalIterations == nil then
        return 1
    end

    return totalIterations * numOfExperiments

end

function _initializeParameterForSimulation()

    Utility.saveTable(configTbl, simname)

    _loadNewParameters()

end

function _setCurrentSimulation( _currentSimulation )

    currentSimulation = _currentSimulation
    configTbl[1].currentSimIteration = _currentSimulation

end

function _loadNewParameters()

    local paramTable = configTbl
    for key, value in pairs(configTbl) do
        if(type(value) == "table") then
            tbl = Utility.handleParameterTable(value, currentSimulation)
            Utility.tableMerge(value, tbl)
        end
    end

    Utility.saveTable( paramTable, parname )

end

function test()

    v1_ss = 0.01
    v1_f = 0
    v1_t = 1.01

    v2_ss = 0.2
    v2_f = 0.0
    v2_t = 100

    v1 = v1_f
    v2 = v2_f

    for i=1,50600 do

        v1 = round(v1 + v1_ss, 5)

        if round(v1 - math.floor(v1 / v1_t) * v1_t, 5) == 0 then
            v2 = round(v2 + v2_ss, 5)
            v1 = round(0, 5)
        end

        print("v1: "..v1.. "\tv2: "..v2)

    end

end






