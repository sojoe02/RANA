//
// Created by sojoe on 1/17/19.
//

#ifndef RANA_FLOWVARIABLES_H
#define RANA_FLOWVARIABLES_H

#include <atomic>


class FlowVariables
{
public:
    static std::atomic_bool flowGenerating;
    static std::atomic_bool flowRunning;
    static std::atomic_bool flowActive;
    static std::atomic_bool flowError;
};

#endif //RANA_FLOWVARIABLES_H
