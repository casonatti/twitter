#ifndef NOTIFICATION_HPP
#include <stdlib.h>
#include <iostream>

class Notification {
    private:
        u_int32_t id;           //notification id
        u_int32_t timestamp;    //notification timestamp
        u_int16_t length;       //message length
        u_int16_t pending;      //qtd readers pending
        const char* _string;    //message

    public:
        void setId(u_int32_t id);

        void setTimestamp(u_int32_t timestamp);
        
        void setLength(u_int16_t length);

        void setPending(u_int16_t pending);

        void setMessage(const char* message);

        u_int32_t getId();

        u_int32_t getTimestamp();

        u_int16_t getLength();

        u_int16_t getPending();

        const char* getMessage();

        void subtractPending();

        void incNotificationId(int* global_notification_id);
};

#endif