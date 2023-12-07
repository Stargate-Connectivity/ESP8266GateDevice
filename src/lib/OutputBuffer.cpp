#include "OutputBuffer.h"

void OutputBuffer::loop() {
    if (this->buffer.size() > 0) {
        String ids = "";
        String lengths = "";
        String values = "";
        for (auto entry : buffer) {
            ids += String(entry.first) + ",";
            String textValue = entry.second->toString();
            lengths += String(textValue.length()) + ",";
            values += textValue;
        }
        ids.remove(ids.length() - 1);
        lengths.remove(lengths.length() - 1);
        this->sendFunction(ids + "|" + lengths + "|" + values);
        this->buffer.clear();
    }
}

void OutputBuffer::sendValue(GateValue* value) {
    this->buffer[value->id] = value;
}

void OutputBuffer::clear() {
    this->buffer.clear();
}