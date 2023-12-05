/*
    ESP8266GateDevice.cpp - Library to connect ESP8266 module with Stargate server
    Created by Michał Marchel, December 2, 2023.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "ESP8266GateDevice.h"

ESP8266GateDevice::ESP8266GateDevice(String ssid, String password) : GateDevice() {
    this->WIFI_SSID = ssid;
    this->WIFI_PASS = password;
};

bool ESP8266GateDevice::startUdp(int port) {
    return this->UDP.begin(10001) == 1;
}

void ESP8266GateDevice::stopUdp() {
    this->UDP.stop();
}

bool ESP8266GateDevice::wasKeywordReceived(char* keyword) {
    int packetSize = this->UDP.parsePacket();
    if (packetSize > 0) {
        char data[packetSize];
        this->UDP.read(data, packetSize);
        return strcmp(data, keyword);
    } else {
        return false;
    }
}

IPAddress ESP8266GateDevice::getServerIp() {
    return this->UDP.remoteIP();
}