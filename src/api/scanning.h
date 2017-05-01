#ifndef SCANNING_H
#define SCANNING_H

#include <unordered_map>
#include <mutex>

#include "src/utility.h"


class Scanning
{
public:
    Scanning();
    static void reset();
    static MatriceInt radialMask(int radius);
    static void edgeMask();

private:

    static std::unordered_map<int, MatriceInt> radialMasks;
    static std::mutex mutex;
};

#endif // SCANNING_H
