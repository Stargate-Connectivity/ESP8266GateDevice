#ifndef ValueFactory_h
#define ValueFactory_h

#include <Arduino.h>
#include <map>
#include "GateValue.h"
#include "GateBool.h"

class ValueFactory
{
    public:
        GateBool* createBool(String direction);

    private:
        std::map<int, GateValue*> valuesMap;
};

#endif