#include "MessageHandler.h"

std::list<String> parseArray(String array) {
    std::list<String> params;
    int separatorIndex = array.indexOf('|');
    String lengths = array.substring(0, separatorIndex);
    String values = array.substring(separatorIndex + 1);
    int nextValueIndex;
    int valueLength;
    while(true) {
        nextValueIndex = lengths.indexOf(',');
        if (nextValueIndex == -1) {
            params.push_back(values);
            break;
        } else {
            valueLength = lengths.substring(0, nextValueIndex).toInt();
            params.push_back(values.substring(0, valueLength));
            lengths = lengths.substring(nextValueIndex + 1);
            values = values.substring(valueLength);
        }
    }
    return params;
}

String createManifest(String deviceName, std::map<int, GateValue*> valuesMap) {
    String manifest("{");
    EEPROM.begin(40);
    delay(10);
    if (EEPROM.read(0) == 0xFF) {
        String id("");
        byte currentByte;
        int currentIndex = 1;
        while(true) {
            currentByte = EEPROM.read(currentIndex);
            if (currentByte == 0) {
                break;
            } else {
                currentIndex++;
                id += (char) currentByte;
            }
        }
        manifest += "\"id\":\"" + id + "\", ";
    }
    EEPROM.end();
    manifest += "\"deviceName\":\"" + deviceName + "\", \"values\":[";
    for (auto valueEntry : valuesMap) {
        manifest += valueEntry.second->toManifest() + ",";
    }
    manifest.remove(manifest.length() - 1);
    manifest += "]}";
    return manifest;
}

void handleIdAssigned(String idAssignedMessage) {
    String idValue = idAssignedMessage.substring(13);
    int separatorIndex = idValue.indexOf('|');
    String id = idValue.substring(separatorIndex + 1);
    int bufferSize = id.length() + 1;
    byte *buffer = new byte[bufferSize];
    id.getBytes(buffer, bufferSize);
    EEPROM.begin(40);
    delay(10);
    EEPROM.write(0, 0xFF);
    for(int i = 0; i < bufferSize; i++) {
        EEPROM.write(i + 1, buffer[i]);
    }
    EEPROM.end();
}