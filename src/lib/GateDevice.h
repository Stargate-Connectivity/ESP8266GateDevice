/*
    ESP8266GateDevice.h - Library to connect ESP8266 module with Stargate server
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

#ifndef GateDevice_h
#define GateDevice_h

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include "MessageHandler.h"

class GateDevice
{
    public:
        GateDevice();
        void setDeviceName(String name);
        void startDevice();
        void loop();
        int connectionState;
        String WIFI_SSID;
        String WIFI_PASS;

    protected:
        virtual bool startUdp(int port) = 0;
        virtual void stopUdp() = 0;
        virtual bool wasKeywordReceived(char* keyword) = 0;
        virtual IPAddress getServerIp() = 0;

    private:
        WebSocketsClient webSocket;
        bool deviceStarted;
        String deviceName;
        void connectServer();
        void webSocketEvent(WStype_t type, uint8_t * payload, size_t length);
        void handlePing();
        unsigned long pingTimer;
        bool pingInProgress;
        int failedPings;
};

#endif