#ifndef GateBool_h
#define GateBool_h

#include <Arduino.h>
#include "GateValue.h"

class GateBool : public GateValue
{
    public:
        GateBool();
        String toString() override;
        void fromRemote(String textValue) override;
        String toManifest() override;
        bool getValue();
        void setValue(bool value);
        String labelTrue;
        String labelFalse;

    private:
        bool value;
        void setRemoteValue(bool value);
};

#endif