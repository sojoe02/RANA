    --  Variables used to set the simulation
simname = "_simconfig.data"
parname = "_parameters.data"

currentSimulation = 0           --  Current simulation
currentExperiments = 1          --  Current experiment
numOfExperiments = 5            --  How many simulations with the same parameters do we want
totalIterations = 100000        --  The total number of simulations ( v1_t/v1_ss * v2_t/v2_ss )

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
        name="greenfield",
        v1_f = 0.0,
        v1_t = 1.0,
        v2_f = 1.0,
        v2_t = 100.0,
        v1_ss = 0.01,
        v2_ss = 0.2,
        numVar=2
    },
    {
        name="master",
        v1_f = 0.0,
        v1_t = 1.0,
        v2_f = 1.0,
        v2_t = 100.0,
        v1_ss = 0.01,
        v2_ss = 0.2,
        numVar=2
    }
}

function _loadNumberIterations()

    if totalIterations == nil then
        return 1
    end

    Utility.saveTable(configTbl, simname)

    _saveNewAgentParametersToFile()

    return totalIterations * numOfExperiments

end

function _saveNewAgentParametersToFile()

    local paramTable = configTbl

    for key, value in pairs(configTbl) do
        if(type(value) == "table") then
            tbl = Utility.handleParameterTable(value, currentSimulation)
            Utility.tableMerge(value, tbl)
        end
    end

    Utility.saveTable( paramTable, parname )
    currentSimulation = currentSimulation + 1
    configTbl[1].simIteration = configTbl[1].simIteration + 1

    print("Simulation #: "..currentSimulation.." of "..totalIterations)
    print("Experiment #: "..currentExperiments.." of "..numOfExperiments)

end








