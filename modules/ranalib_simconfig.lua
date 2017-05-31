    --  Variables used to set the simulation
simname = "_simconfig.data"
parname = "_parameters.data"

currentSimulation = 0       --  Current simulation
totalIterations = 10         --  The total number of simulations
currentExperiments = 1          --  Current experiment
numOfExperiments = 1            --  How many simulations with the same parameters do we want


--  Import Rana lua libraries
Utility = require "ranalib_utility"

--  Master table to be set with parameters
configTbl =
{
    {
        name="simulation",
        simIteration=1, --Which simulation are we doing.
        expIteration=1, --How many times have we run the same experiment
        numAgents=2
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

function _loadInitializeSimulation()

    if totalIterations == nil then
        return 1
    end

    Utility.saveTable(configTbl, simname)

    _saveNewAgentParametersToFile()

    return totalIterations * numOfExperiments

end

function _saveNewAgentParametersToFile()
    currentSimulation = currentSimulation + 1

    local paramTable = configTbl
    for key, value in pairs(configTbl) do
        if(type(value) == "table") then
            tbl = Utility.handleParameterTable(value, currentSimulation)
            Utility.tableMerge(value, tbl)
        end
    end

    Utility.saveTable( paramTable, parname )
    configTbl[1].simIteration = configTbl[1].simIteration + 1

    print("Simulation #: "..currentSimulation.." of "..totalIterations)
    print("Experiment #: "..currentExperiments.." of "..numOfExperiments)
    print("\tVar1: "..paramTable[2].v1.." to "..paramTable[2].v1_t-paramTable[2].v1_ss)
    print("\tVar2: "..paramTable[2].v2.." to "..paramTable[2].v2_t)

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






