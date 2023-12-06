#ifndef ValueFactory_h
#define ValueFactory_h

#include <Arduino.h>
#include <map>
#include "GateValue.h"
#include "GateBool.h"
#include "../OutputBuffer.h"

class ValueFactory
{
    public:
        GateBool* createBool(String direction);
        std::map<int, GateValue*> getValues();
        OutputBuffer* outputBuffer;

    private:
        std::map<int, GateValue*> valuesMap;
};

#endif