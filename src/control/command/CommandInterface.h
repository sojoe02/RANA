//
// Created by sojoe on 1/16/19.
//

#ifndef RANA_COMMANDINTERFACE_H
#define RANA_COMMANDINTERFACE_H

#include "simulator/core/FlowControl.h"
#include "globals/RanaDefinitions.hpp"


class CommandInterface
{
public:
    FlowControl *generateFlow(Definitions::SimInfo simInfo);
};


#endif //RANA_COMMANDINTERFACE_H
