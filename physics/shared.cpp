#include <climits>

#include <iostream>
#include "shared.h"

std::map<std::string, double> *Shared::sharedNumbers = NULL;

Shared::Shared()
{
}

void Shared::initShared()
{
    if (sharedNumbers != NULL)
    {
        sharedNumbers->clear();
    } else
        sharedNumbers = new std::map<std::string, double>();
}

void Shared::addNumber(std::string key, double value)
{
    sharedNumbers->insert(std::pair<std::string, double>(key, value));
}

double Shared::getNumber(std::string key)
{
    if(sharedNumbers->find(key) != sharedNumbers->end())
    {
        return sharedNumbers->find(key)->second;
    } else return LLONG_MIN;
}



