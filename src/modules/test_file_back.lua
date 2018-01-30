--  Import tables, agents, sim, and param.
local utility = require "ranalib_utility"
local Shared = require "ranalib_shared"


--  Set local tables used
local simParam = {}
local paramOrderedKeys = {}
local agentOrderedKeys = {}

local function _testParamForLoop(table, numInTable, maxNumInTable)

    --  Base case
    if numInTable > maxNumInTable then

        for index, value in ipairs(simParam) do
            Shared.storeNumber(index, value)
            --io.write(value, ' ')    --  TODO: Outcomment
        end
        --io.write('\n')  --  TODO: Outcomment

        coroutine.yield()

        return
    end

    --  Nested case
    local k, v = paramOrderedKeys[numInTable], param[ paramOrderedKeys[numInTable] ]
    if #v == 2 then
        for p = v[1], v[2] do
            simParam[numInTable] = p
            _testParamForLoop(table, numInTable+1, maxNumInTable)
        end
    elseif #v == 3 then
        if v[3] > v[2] then
            v[2], v[3] = v[3], v[2]
        end

        for p = v[1], v[2], v[3] do
            simParam[numInTable] = p
            _testParamForLoop(table, numInTable+1, maxNumInTable)
        end
    else
        for p = 1, #v do
            simParam[numInTable] = v[p]
            _testParamForLoop(table, numInTable+1, maxNumInTable)
        end
    end

    return 0
end


function _testParamMain()

    paramOrderedKeys = {}
    for value in pairs(param) do
        table.insert(paramOrderedKeys, value)
        --print(paramOrderedKeys, value, #paramOrderedKeys)
    end

    co = coroutine.create( function() _testParamForLoop(param, 1, #paramOrderedKeys) end)

    return 0
end

function _testParamMainCo()
    coroutine.resume(co)

    if coroutine.status(co) ~= "dead" then
        return true
    end

    return false
end

local function agentMain()
    local _key = 0
    local _path
    local _num

    for value = 1, #agents do
        --  Check if it is a file path
        if type(agents[value]) == "string" then
            _key = _key+1
            _path = agents[value]

            io.write(agents[value], " ")

            --  Check if there is a number of agents to initiate
            if type( agents[value+1] ) == "number" then
                _num = agents[value+1]
               io.write( select(2, next( agents, key)) )
            else    --  Instantiate 1
                _num = 1
               io.write( 1 )

            end
            Shared.storeAgent(_key, _path, _num)
            io.write("\n")
            print(_key, _path, _num)
        end
        Shared.storeNumber("numAgents", _key)
    end

end

function _getSimulationFile(inputFilePath)
    require(inputFilePath)
    print(inputFilePath)
end

function _checkIfInputFileIsSimulationType()
    print("2")
    if type(sim) == "table" and
        type(agents) == "table" and
         type(param) == "table" then
                return true
    end

    return false
end

function _testFunc()
    agentMain()
    --paramMain() -- This is done.
end


























