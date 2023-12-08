#include "ValueFactory.h"

GateBool ValueFactory::createBool(String direction) {
    GateBool value(this->outputBuffer);
    value.direction = GateValue::getDirection(direction);
    this->valuesMap[value.id] = &value;
    return value;
}

GateInt ValueFactory::createInt(String direction) {
    GateInt value(this->outputBuffer);
    value.direction = GateValue::getDirection(direction);
    this->valuesMap[value.id] = &value;
    return value;
}

std::map<int, GateValue*> ValueFactory::getValues() {
    return this->valuesMap;
}