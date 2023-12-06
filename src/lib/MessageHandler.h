#ifndef MessageHandler_h
#define MessageHandler_h

#include <list>
#include <Arduino.h>
#include <EEPROM.h>
#include <map>
#include "values/GateValue.h"

std::list<String> parseArray(String array);
String createManifest(String deviceName, std::map<int, GateValue*> valuesMap);
void handleIdAssigned(String idAssignedMessage);

#endif