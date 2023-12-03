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
    this->WIFI_SSID = ssid;
    this->WIFI_PASS = password;
    this->deviceStarted = false;
    this->deviceName = "";
    this->connectionState = 0;
};

void ESP8266GateDevice::setDeviceName(String name) {
    if (!this->deviceStarted) {
        this->deviceName = name;
    }
};

void ESP8266GateDevice::startDevice() {
    if (!this->deviceStarted) {
        this->deviceStarted = true;
        WiFi.begin(this->WIFI_SSID, this->WIFI_PASS);
        while (WiFi.status() != WL_CONNECTED) {
            delay(100);
        }
        this->connectServer();
    }
};

void ESP8266GateDevice::loop() {
    if (WiFi.status() == WL_CONNECTED) {
        if (this->connectionState == 4) {
            // TODO
        } else {
            this->connectServer();
        }
    } else {
        this->connectionState = 0;
    }
}

void ESP8266GateDevice::connectServer() {
    switch (this->connectionState) {
        case 0:
        Serial.println("Discovery...");
        {
            this->webSocket.disconnect();
            int result = this->UDP.begin(10001);
            if (result == 1) {
                this->connectionState = 1;
            }
            break;
        }
        case 1:
        {
            int packetSize = this->UDP.parsePacket();
            if (packetSize > 0) {
                Serial.println("Connecting...");
                char data[packetSize];
                this->UDP.read(data, packetSize);
                if (strcmp(data, "GateServer")) {
                    IPAddress serverIp = UDP.remoteIP();
                    this->UDP.stop();
                    this->webSocket.begin(serverIp, 10003, "/");
                    this->webSocket.onEvent(std::bind(&ESP8266GateDevice::webSocketEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
                    this->connectionState = 2;
                }
            }
            break;
        }
        case 2:
        case 3:
        {
            this->webSocket.loop();
            break;
        }
    }
}

void ESP8266GateDevice::webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.println("Socket closed");
            this->connectionState = 0;
            break;
        case WStype_CONNECTED:
            Serial.println("Socket ready");
            this->connectionState = 3;
            break;
        case WStype_TEXT:
//            Serial.println(payload);
//            USE_SERIAL.printf("[WSc] get text: %s\n", payload);

            // send message to server
            // webSocket.sendTXT("message here");
            break;
    }
}