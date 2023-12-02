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

#ifndef ESP8266GateDevice_h
#define ESP8266GateDevice_h

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

class ESP8266GateDevice
{
    public:
        ESP8266GateDevice(String ssid, String password);
        void setDeviceName(String name);
        void startDevice();
        void handle();
        int connectionState;

    private:
        String WIFI_SSID;
        String WIFI_PASS;
        WiFiUDP UDP;
        bool deviceStarted;
        String deviceName;
        void connectServer();
};

#endif