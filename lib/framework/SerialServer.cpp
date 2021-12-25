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
#include "SerialServer.h"

SerialServer::SerialServer() {
    this->connect(std::string{"Serial"}, &this->serial);
    this->connect(std::string{"DebugSerial"}, &Serial);
}

void SerialServer::begin() {
    Serial.println("Setting up serial service on pins %s"+String(this->rxPin)+" and "+String(this->txPin)+"baud: "+String(this->baud)+"config: "+String(this->config, HEX));
    this->serial.begin(this->baud,this->config,this->rxPin,this->txPin,this->invert,this->timeout_ms);
    if (this->baud == 0) {
        this->baud = this->serial.baudRate();
        if (this->baud != 0) {
            Serial.println("No baudrate set, detected baudrate: "+String(this->baud));
        };
    };
}

Stream* SerialServer::connect(std::string name) {
        this->clients_.push_back(std::unique_ptr<PipedStreamPair>(new PipedStreamPair));
        this->connect(name, &this->clients_.back()->first);
        return &this->clients_.back()->second;
}

void SerialServer::loop() {
    this->write();
}

void SerialServer::end() {
    Serial.println("Stopping serial service");
    this->serial.end();
}

// void SerialServer::cleanup() {
//     auto discriminator = [](std::unique_ptr<Client> &client) { return !client->disconnected; };
//     auto last_client = std::partition(this->clients_.begin(), this->clients_.end(), discriminator);
//     for (auto it = last_client; it != this->clients_.end(); it++)
//         Serial.println("Client %s disconnected"+String((*it)->identifier.c_str()));

//     this->clients_.erase(last_client, this->clients_.end());
// }

// }

