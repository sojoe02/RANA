#ifndef SCANNING_H
#define SCANNING_H

#include <unordered_map>

#include "utility.h"


class Scanning
{
public:
    Scanning();
    static void reset();
    static MatriceInt radialMask(int radius);

private:

    static std::unordered_map<int, MatriceInt> radialMasks;
};

#endif // SCANNING_H
