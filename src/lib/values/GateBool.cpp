#include "GateBool.h"

GateBool::GateBool(OutputBuffer* outputBuffer) : GateValue() {
    this->value = false;
    this->type = "boolean";
    this->outputBuffer = outputBuffer;
}

String GateBool::toString() {
    return this->value ? "1" : "0";
}

void GateBool::fromRemote(String textValue) {
    this->setRemoteValue(textValue.equals("1"));
}

String GateBool::toManifest() {
    String manifest = this->toPartialManifest();
    if ((this->labelTrue.length() > 0) || (this->labelFalse.length() > 0)) {
        manifest += ",\"options\":{\"labelTrue\":\"" + this->labelTrue + "\",\"labelFalse\":\"" + this->labelFalse + "\"}}";
    } else {
        manifest += "}";
    }
    return manifest;
}

bool GateBool::getValue() {
    return this->value;
}

void GateBool::setValue(bool value) {
    if(this->value != value) {
        this->value = value;
        this->outputBuffer->sendValue(this);
    }
}

void GateBool::setRemoteValue(bool value) {
    this->value = value;
}