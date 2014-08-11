#ifndef SHARED_H
#define SHARED_H

#include <string>
#include <map>

class Shared
{
public:
    Shared();

    static std::map<std::string, double> *sharedNumbers;

    static double getNumber(std::string key);
    static void addNumber(std::string key, double value);
    static void initShared();
};

#endif // SHARED_H
