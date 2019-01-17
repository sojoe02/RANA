//
// Created by sojoe on 1/16/19.
//

#include "CommandInterface.h"

FlowControl *CommandInterface::generateFlow(Definitions::SimInfo simInfo)
{
    if (simInfo.sourceType == Definitions::SourceType::LUA)
    {
        //load the source info..from the lua command interface implementation! //todo
    }

    return nullptr;
}