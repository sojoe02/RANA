//
// Created by sojoe on 1/17/19.
//

#ifndef RANADEFINITIONS_HPP
#define RANADEFINITIONS_HPP

#include <string>

namespace Definitions
{
    enum SourceType
    {
        LUA, CPLUSPLUS
    };

    struct SimInfo
    {
        Definitions::SourceType sourceType;
        std::string luaChunk;
    };
};

#endif //RANADEFINITIONS_HPP
