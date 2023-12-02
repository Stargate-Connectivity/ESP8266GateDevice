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

ESP8266GateDevice::ESP8266GateDevice(String ssid, String password) {
    WIFI_SSID = ssid;
    WIFI_PASS = password;
    deviceStarted = false;
    deviceName = "";
    connectionState = 0;
};

void ESP8266GateDevice::setDeviceName(String name) {
    if (!deviceStarted) {
        deviceName = name;
    }
};

void ESP8266GateDevice::startDevice() {
    if (!deviceStarted) {
        deviceStarted = true;
        WiFi.begin(WIFI_SSID, WIFI_PASS);
        while (WiFi.status() != WL_CONNECTED) {
            delay(100);
        }
        connectServer();
    }
};

void ESP8266GateDevice::handle() {
    if (WiFi.status() == WL_CONNECTED) {
        if (connectionState == 2) {
            // TODO
        } else {
            connectServer();
        }
    } else {
        connectionState = 0;
    }
}

void ESP8266GateDevice::connectServer() {
    if (connectionState == 0) {
        delete UDP;
        UDP = new WiFiUDP();
        int result = UDP->begin(10001);
        if (result == 1) {
            connectionState = 1;
        } else {
            return;
        }
    } else {
        int packetSize = UDP->parsePacket();
        if (packetSize > 0) {
            char data[packetSize];
            UDP->read(data, packetSize);
            if (strcmp(data, "GateServer")) {
                IPAddress serverIp = UDP->remoteIP();
                UDP->stop();
                delete UDP;
                connectionState = 2;
            }
        }
    }
}