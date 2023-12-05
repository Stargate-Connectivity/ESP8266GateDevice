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
        this->onDeviceStart();
        this->connectServer();
    }
};

void GateDevice::loop() {
    if (this->networkAvailable()) {
        if (this->connectionState == 4) {
            this->loopSocket();
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
            this->stopSocket();
            bool success = this->startUdp(10001);
            if (success) {
                this->connectionState = 1;
            }
            break;
        }
        case 1:
        {
            if (this->wasKeywordReceived((char*) "GateServer")) {
                String serverIp = this->getServerIp();
                this->stopUdp();
                this->startSocket(serverIp, 10003);
                this->connectionState = 2;
            }
            break;
        }
        case 2:
        case 3:
        {
            this->loopSocket();
            break;
        }
    }
}

void GateDevice::handlePing() {
    unsigned long now = millis();
    if (this->pingTimer < now) {
        if (this->pingInProgress) {
            this->failedPings++;
            if (this->failedPings > 3) {
                this->stopSocket();
                this->connectionState = 0;
            }
        }
        if (this->connectionState == 4) {
            this->pingInProgress = true;
            this->pingTimer = now + 1000;
            this->send("*?ping");
        }
    }
}

void GateDevice::socketOpened() {
    this->connectionState = 3;
}

void GateDevice::socketClosed() {
    this->connectionState = 0;
}

void GateDevice::onMessage(char* message) {
    if (this->connectionState == 3) {
        if (message[0] == '*') {
            if (message[1] == '?') {
                message[1] = '>';
                String response = strcat(message, "|");
                response += createManifest(this->deviceName);
                this->send(response);
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
}