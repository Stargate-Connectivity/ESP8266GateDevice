#include "GateValue.h"

int GateValue::nextId = 1;

GateValue::GateValue() {
    this->id = GateValue::nextId;
    GateValue::nextId++;
}

String GateValue::toPartialManifest() {
    String manifest = "{\"id\":\"" + String(this->id)
            + "\",\"type\":\"" + this->type
            + "\",\"valueName\":\"" + this->valueName
            + "\",\"direction\":\"" + this->direction
            + "\"";
    return manifest;
}

