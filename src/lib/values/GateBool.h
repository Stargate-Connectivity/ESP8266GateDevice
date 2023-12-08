#ifndef GateBool_h
#define GateBool_h

#include <Arduino.h>
#include "GateValue.h"
#include "../OutputBuffer.h"

class GateBool : public GateValue
{
    public:
        GateBool(OutputBuffer* outputBuffer);
        String toString() override;
        void fromRemote(String textValue) override;
        String toManifest() override;
        bool getValue();
        void setValue(bool value);
        String labelTrue;
        String labelFalse;

    private:
        OutputBuffer* outputBuffer;
        bool value;
};

#endif