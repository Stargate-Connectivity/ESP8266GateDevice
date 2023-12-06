#include "ValueFactory.h"

GateBool* ValueFactory::createBool(String direction) {
    GateBool* value = new GateBool();
    value->direction = direction;
    this->valuesMap[value->id] = value;
    return value;
}