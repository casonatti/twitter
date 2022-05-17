#include "Packet.hpp"
#include <stdlib.h>
#include <iostream>
#include <string>
#include <string.h>

using namespace std;

void Packet::setType(u_int16_t type) {
    this->type = type;
}

void Packet::setSeqn(u_int16_t seqn) {
    this->seqn = seqn;
}

void Packet::setLength(u_int16_t length) {
    this->length = length;
}

void Packet::setTimestamp(u_int16_t timestamp) {
    this->timestamp = timestamp;
}

void Packet::setPayload(const char* payload) {
    this->_payload = payload;
}

u_int16_t Packet::getType() {
    return this->type;
}

u_int16_t Packet::getSeqn() {
    return this->seqn;
}

u_int16_t Packet::getLength() {
    return this->length;
}

u_int16_t Packet::getTimestamp() {
    return this->timestamp;
}

const char* Packet::getPayload() {
    return this->_payload;
}

void Packet::serialize(std::string* str_serialization) {
    const char token = '&';

    *str_serialization += std::to_string(this->getType());   
    *str_serialization += token;
    *str_serialization += std::to_string(this->getSeqn());
    *str_serialization += token;
    *str_serialization += std::to_string(this->getLength());
    *str_serialization += token;
    *str_serialization += std::to_string(this->getTimestamp());
    *str_serialization += token;
    *str_serialization += std::to_string(this->getLength());
    *str_serialization += token;
    *str_serialization += this->getPayload();
    *str_serialization += '\0';
}

Packet Packet::unserialize() {
    Packet pack_unserialized;

    //TODO unserialization...

    return pack_unserialized;
}