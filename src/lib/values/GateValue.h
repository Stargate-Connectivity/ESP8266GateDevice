#ifndef GateValue_h
#define GateValue_h

#include <Arduino.h>

class GateValue
{
    public:
        GateValue();
        int id;
        String valueName;
        String direction;
        virtual String toString() = 0;
        virtual void fromRemote(String textValue) = 0;
        virtual String toManifest() = 0;

    protected:
        String type;
        String toPartialManifest();
        void send();

    private:
        static int nextId;
};

#endif