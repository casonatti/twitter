#include "Notification.hpp"

void Notification::setId(uint32_t id) {
    this->id = id;
}

void Notification::setTimestamp(u_int32_t timestamp) {
    this->timestamp = timestamp;
}


//TODO this method doesn't look ok
void Notification::setLength() {
    u_int16_t length;

    length = sizeof(this->getId()) + sizeof(this->getTimestamp()) + strlen(this->getMessage());

    this->length = length;
}

void Notification::setPending(u_int16_t pending) {
    this->pending = pending;
}

void Notification::setMessage(const char* message) {
    this->_string = message;
}

u_int32_t Notification::getId() {
    return this->id;
}

u_int32_t Notification::getTimestamp() {
    return this->timestamp;
}

u_int16_t Notification::getLength() {
    return this->length;
}

u_int16_t Notification::getPending() {
    return this->pending;
}

const char* Notification::getMessage() {
    return this->_string;
}

void Notification::subtractPending() {
    this->pending--;
}

void Notification::incNotificationId(int* global_notification_id) {
    *global_notification_id++;
}

void Notification::serialize(std::string* str_serialization) {
    const char token = '&';

    *str_serialization += std::to_string(this->getId());   
    *str_serialization += token;
    *str_serialization += std::to_string(this->getTimestamp());
    *str_serialization += token;
    *str_serialization += std::to_string(this->getLength());
    *str_serialization += token;
    *str_serialization += this->getMessage();
}

Notification Notification::unserialize(const char* notification_serialized) {
    Notification notification_unserieliazed;

    return notification_unserieliazed;
}