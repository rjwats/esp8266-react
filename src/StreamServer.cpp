/* Copyright (C) 2020 Oxan van Leeuwen
 *
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

#include "StreamServer.h"

void StreamServer::setup() {
    Serial.println("Setting up stream server...");
    this->stream_.begin(this->baud,this->config,this->rx_pin);
    this->recv_buf_.reserve(128);

    this->server_ = AsyncServer(this->port_);
    this->server_.begin();
    this->server_.onClient([this](void *h, AsyncClient *tcpClient) {
        if(tcpClient == nullptr)
            return;

        this->clients_.push_back(std::unique_ptr<Client>(new Client(tcpClient, this->recv_buf_)));
    }, this);
}

void StreamServer::loop() {
    this->cleanup();
    this->read();
    this->write();
}

void StreamServer::cleanup() {
    auto discriminator = [](std::unique_ptr<Client> &client) { return !client->disconnected; };
    auto last_client = std::partition(this->clients_.begin(), this->clients_.end(), discriminator);
    for (auto it = last_client; it != this->clients_.end(); it++)
        Serial.println("Client %s disconnected"+String((*it)->identifier.c_str()));

    this->clients_.erase(last_client, this->clients_.end());
}

void StreamServer::read() {
    int len;
    while ((len = this->stream_.available()) > 0) {
        char buf[128];
        size_t read = this->stream_.readBytes(buf, min(len, 128));
        for (auto const& client : this->clients_)
            client->tcp_client->write(buf, read);
    }
}

void StreamServer::write() {
    size_t len;
    while ((len = this->recv_buf_.size()) > 0) {
        this->stream_.write(this->recv_buf_.data(), len);
        this->recv_buf_.erase(this->recv_buf_.begin(), this->recv_buf_.begin() + len);
    }
}

void StreamServer::dump_config() {
    // Serial.println(TAG, "Stream Server:");
    // Serial.println(TAG, "  Address: %s:%u", network_get_address().c_str(), this->port_);
}

void StreamServer::on_shutdown() {
    for (auto &client : this->clients_)
        client->tcp_client->close(true);
}

StreamServer::Client::Client(AsyncClient *client, std::vector<uint8_t> &recv_buf) :
        tcp_client{client}, identifier{client->remoteIP().toString().c_str()}, disconnected{false} {
    Serial.println("New client connected from %s"+String(this->identifier.c_str()));

    this->tcp_client->onError(     [this](void *h, AsyncClient *client, int8_t error)  { this->disconnected = true; });
    this->tcp_client->onDisconnect([this](void *h, AsyncClient *client)                { this->disconnected = true; });
    this->tcp_client->onTimeout(   [this](void *h, AsyncClient *client, uint32_t time) { this->disconnected = true; });

    this->tcp_client->onData([&](void *h, AsyncClient *client, void *data, size_t len) {
        if (len == 0 || data == nullptr)
            return;

        auto buf = static_cast<uint8_t *>(data);
        recv_buf.insert(recv_buf.end(), buf, buf + len);
    }, nullptr);
}

StreamServer::Client::~Client() {
    delete this->tcp_client;
}