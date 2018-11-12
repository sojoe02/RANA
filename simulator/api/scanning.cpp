#include <math.h>

#include "phys.h"
#include "scanning.h"
#include "simulator/utility/utility.h"
#include "simulator/output.h"

std::unordered_map<int, MatriceInt> Scanning::radialMasks;
std::mutex Scanning::mutex;

//Scanning::Scanning()
//{

//}

void Scanning::reset() {
    radialMasks.clear();
}

MatriceInt Scanning::radialMask(int radius) {
    int range = radius * 2;
    std::vector<std::vector<int> > tmpvec1(range, std::vector<int>(range));
    MatriceInt result = tmpvec1;

    //if(image == NULL)
    //  return result;

    if (Scanning::radialMasks.find(radius) == Scanning::radialMasks.end()) {
        std::lock_guard<std::mutex> guard(mutex);
        //calculate new mask:
        for (int x = 0; x < radius; x++) {
            for (int y = 0; y < radius; y++) {
                double distance = sqrt(pow((double) x, 2) + pow((double) y, 2));

                //Output::Inst()->kprintf("%f,d", distance, radius);

                //if the distance is smaller than the actual radius.
                if (distance + .5 < radius) {
                    result[radius + x][radius + y] = 1;
                    result[radius - x][radius + y] = 1;
                    result[radius - x][radius - y] = 1;
                    result[radius + x][radius - y] = 1;

                } else {
                    result[radius + x][radius + y] = 0;
                    result[radius - x][radius + y] = 0;
                    result[radius - x][radius - y] = 0;
                    result[radius + x][radius - y] = 0;
                }

            }
        }
        Scanning::radialMasks[radius] = result;
    }
    return Scanning::radialMasks[radius];
}

