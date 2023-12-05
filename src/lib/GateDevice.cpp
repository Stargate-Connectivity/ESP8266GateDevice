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

#include "GateDevice.h"

GateDevice::GateDevice() {
    this->deviceStarted = false;
    this->deviceName = "";
    this->connectionState = 0;
    this->pingTimer = 0;
    this->pingInProgress = false;
    this->failedPings = 0;
};

void GateDevice::setDeviceName(String name) {
    if (!this->deviceStarted) {
        this->deviceName = name;
    }
};

void GateDevice::startDevice() {
    if (!this->deviceStarted) {
        this->deviceStarted = true;
        WiFi.begin(this->WIFI_SSID, this->WIFI_PASS);
        while (WiFi.status() != WL_CONNECTED) {
            delay(100);
        }
        this->connectServer();
    }
};

void GateDevice::loop() {
    if (WiFi.status() == WL_CONNECTED) {
        if (this->connectionState == 4) {
            this->webSocket.loop();
            this->handlePing();
            // TODO
        } else {
            this->connectServer();
        }
    } else {
        this->connectionState = 0;
    }
}

void GateDevice::connectServer() {
    switch (this->connectionState) {
        case 0:
        {
            this->webSocket.disconnect();
            bool success = this->startUdp(10001);
            if (success) {
                Serial.println("UDP started");
                this->connectionState = 1;
            }
            break;
        }
        case 1:
        {
            if (this->wasKeywordReceived((char*) "GateServer")) {
                IPAddress serverIp = this->getServerIp();
                this->stopUdp();
                this->webSocket.begin(serverIp, 10003, "/");
                this->webSocket.onEvent(std::bind(&GateDevice::webSocketEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
                this->connectionState = 2;
                Serial.println("Socket started");
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

void GateDevice::webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.println("Socket closed");
            this->connectionState = 0;
            break;
        case WStype_CONNECTED:
            Serial.println("Socket opened");
            this->connectionState = 3;
            break;
        case WStype_TEXT:
            char * message = (char *) payload;
            if (this->connectionState == 3) {
                if (message[0] == '*') {
                    if (message[1] == '?') {
                        message[1] = '>';
                        String response = strcat(message, "|");
                        response += createManifest(this->deviceName);
                        this->webSocket.sendTXT(response);
                    } else if (message[1] == '!') {
                        if (message[2] == 'a') {
                            String msg(message);
                            handleIdAssigned(msg);
                        } else if (message[2] == 'r') {
                            this->pingInProgress = false;
                            this->failedPings = 0;
                            this->connectionState = 4;
                        }
                    }
                }
            } else {
                if (message[0] == '*') {
                    if (message[1] == '>' && this->pingInProgress) {
                        this->pingInProgress = false;
                        this->pingTimer = millis() + 3000;
                        this->failedPings = 0;
                    }
                }
            }
            break;
    }
}

void GateDevice::handlePing() {
    unsigned long now = millis();
    if (this->pingTimer < now) {
        if (this->pingInProgress) {
            this->failedPings++;
            if (this->failedPings > 3) {
                this->webSocket.disconnect();
                this->connectionState = 0;
            }
        }
        if (this->connectionState == 4) {
            this->pingInProgress = true;
            this->pingTimer = now + 1000;
            this->webSocket.sendTXT("*?ping");
        }
    }
}