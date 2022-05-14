#ifndef PACKET_HPP
#include <stdlib.h>

class Packet {
    private:
        u_int16_t type;
        u_int16_t seqn;
        u_int16_t length;
        u_int16_t timestamp;
        const char* _payload;
    
    public:
        void setType(u_int16_t type);

        void setSeqn(u_int16_t seqn);

        void setLength(u_int16_t length);

        void setTimestamp(u_int16_t timestamp);

        void setPayload(const char* payload);

        u_int16_t getType();

        u_int16_t getSeqn();

        u_int16_t getLength();

        u_int16_t getTimestamp();

        const char* getPayload();
};

#endif