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

#ifndef StreamBus_h
#define StreamBus_h

#include <memory>
#include <algorithm>
#include <string>
#include <vector>
#include <Stream.h>
#include <PipedStream.h>
#include <HardwareSerial.h>

class StreamBus {
public:
    void write();
    bool connect(std::string name, Stream* stream);
    bool disconnect(std::string name);
    bool disconnect(Stream* stream);

protected:


    struct Node {
        Node(std::string name, Stream* stream);

        Stream* stream;
        std::string name{};
        bool disconnected{false};
    };

    std::vector<std::unique_ptr<Node>> nodes_{};
};
#endif // end StreamBus_h