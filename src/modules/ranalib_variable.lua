--[[
--  This file is used by the agents,
--  when they needs to set their parameters

function setParameters( table, currentIteration )
    --  Go in and look at how many variables that needs to be used.

    print(table.v1_t)

    if table.numVar == 1 then
        print("ranalib_var - 1")
        return singleParam( table )
    elseif table.numVar == 2 then
        print("ranalib_var - 2")
        return doubleParam( table )
    elseif table.numVar > 2 then
        print("ranalib_var - >2")
        return dynamicParam( table )
    else
        print("There are no variables to set")
    end
end

function singleParam( tbl, currentIteration )
    print(tbl.numIterations)

    return tbl.stepSize * math.fmod( currentIteration, tbl.numIterations )
end

function doubleParam( tbl, currentIteration )
    return{
    tbl.stepSizeV1 * math.floor( currentIteration / tbl.numIterationsV1 ),
    tbl.stepSizeV2 * math.fmod( currentIteration, tbl.numIterationsV2 )
    }
end

function dynamicParam( tbl, currentIteration )

end








#include <iostream>
#include <vector>

std::vector<std::vector<int> > var = {
    {0,1,2,3,4,5,6,7,8,9,10},
    {0,1,2,3,4,5,6,7,8,9,10}
};

int productSize(std::vector<std::vector<int> > &var, int start, int end){
    int pro = 1;
    for (int i = start; i < end; i++){
        pro *= var[i].size();
    }
    return pro;
}

int main(){
    std::cout << productSize(var, 0, var.size()) << std::endl;
    for (int i = 0; i < productSize(var, 0, var.size()); i++){
        for (int j = 0; j < var.size(); j++){
            std::cout << i / productSize(var, j + 1, var.size()) % var[j].size() << ", ";
        }
        std::cout << std::endl;
    }
}
]]--
