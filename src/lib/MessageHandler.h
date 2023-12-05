#ifndef MessageHandler_h
#define MessageHandler_h

#include <list>
#include <Arduino.h>
#include <EEPROM.h>

std::list<String> parseArray(String array);
String createManifest(String deviceName);
void handleIdAssigned(String idAssignedMessage);

#endif