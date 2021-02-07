/* Copyright (C) 2021 Maarten Kleijwegt

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef SerialServer_h
#define SerialServer_h

#include <memory>
#include <string>
#include <vector>
#include <Stream.h>
#include <PipedStream.h>
#ifdef ESP32
#include <HardwareSerial.h>
#elif defined(ESP8266)
#include <SoftwareSerial.h>
#endif
#include <StreamBus.h>

class SerialServer : private StreamBus {
public:
    SerialServer();
    void begin();
    void end();
    void loop() ;
    //void onShutdown() ;
    using StreamBus::connect;
    Stream* connect(std::string name); 

    void setBaud(unsigned long baud) { this->baud = baud; };
    void setRxPin(uint8_t rxPin) { this->rxPin = rxPin; };
    void setTxPin(uint8_t txPin) { this->txPin = txPin; };
    void setInvert(bool invert) { this->invert = invert; };
    void setConfig(uint32_t config) { this->config = config; };
    void setTimeout_ms(unsigned long timeout_ms) { this->timeout_ms = timeout_ms; };

    HardwareSerial serial{2};
    uint8_t rxPin{15};
    uint8_t txPin{17};
    unsigned long baud{0};
    uint32_t config{SERIAL_8N1};
    bool invert{false};
    unsigned long timeout_ms{15000};

protected:
    void cleanup();

    std::vector<std::unique_ptr<PipedStreamPair>> clients_{};
};

#endif // end SerialServer_h
#ifndef PipedStreamPairShared_h
#define PipedStreamPairShared_h

class PipedStreamPairShared : private PipedStreamPair {
    std::shared_ptr<PipedStream> left{std::make_shared<PipedStream>(first)}, right{std::make_shared<PipedStream>(second)};
};

#endif // end PipedStreamPairShared_h