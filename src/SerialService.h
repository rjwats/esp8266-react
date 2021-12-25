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

#pragma once

#include <memory>
#include <string>
#include <vector>
#include <Stream.h>
#include <HardwareSerial.h>
#include <PipedStream.h>

class SerialService {
public:
    void setup();
    void loop() ;
    void dump_config() ;
    void on_shutdown() ;
    Stream

    void set_pin(uint8_t pin) { this->pin_ = pin; }
    void set_uart(uint8_t uart) { this->uart_ = uart; }

protected:
    void cleanup();
    void read();
    void write();

    struct Client {
        Client(Stream *stream, std::vector<uint8_t> &recv_buf);
        ~Client();

            Stream *stream_;
        std::string identifier{};
        bool disconnected{false};
    };

    uint8_t uart_{2}
    uint8_t pin_{26};
    std::vector<uint8_t> recv_buf_{};
    std::vector<std::unique_ptr<Client>> clients_{};
    unsigned long baud{115200};
    uint32_t config{SERIAL_8N1};
};