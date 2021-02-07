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
#include <StreamBus.h>
bool StreamBus::connect(std::string name, Stream* stream) {
        this->nodes_.push_back(
            std::unique_ptr<Node>(
                new Node(
                    name,
                    stream
                )
            )
        );
        return true;
}


bool StreamBus::disconnect(Stream* stream) {
    auto it = std::find_if (this->nodes_.begin(),this->nodes_.end(), 
    [&] (std::unique_ptr<Node> const& node) { return node->stream == stream; });
    if (it != this->nodes_.end()) {
        this->nodes_.erase(it);
        return true;
    }
    return false;
}

bool StreamBus::disconnect(std::string name) {
    auto it = std::find_if (this->nodes_.begin(),this->nodes_.end(), 
    [&] (std::unique_ptr<Node> const& node) { return node->name == name; });
    if (it != this->nodes_.end()) {
        this->nodes_.erase(it);
        return true;
    }
    return false;
}

void StreamBus::write() {
    size_t len;
    for (auto const& source : this->nodes_) {
        while((len = source->stream->available()) > 0) {
            char buf[128];
            size_t read = source->stream->readBytes(buf, std::min(len, size_t{128}));
            for (auto const& node : this->nodes_) {
                if (source != node) {
                    node->stream->write(buf, read);
                }
            
            }
        }
    }
}

StreamBus::Node::Node(std::string name, Stream* stream) :
        stream{stream}, name{name}, disconnected{false} {
    Serial.println("Streambus: New node %s"+String(this->name.c_str())+" connected");

}