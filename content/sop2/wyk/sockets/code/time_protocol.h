#ifndef TIME_PROTOCOL_H
#define TIME_PROTOCOL_H

#include <stdint.h>

#define MSG_TYPE_REQ 1
#define MSG_TYPE_RES 2

/* * __attribute__((packed)) forces the compiler not to add padding bytes.
 * This guarantees our app-layer header is exactly 13 bytes across all platforms.
 */
struct __attribute__((packed)) time_msg {
    uint8_t  msg_type;      // 1 = Request, 2 = Response
    uint32_t seq_id;        // Sequence Number
    uint64_t timestamp_ms;  // Unix timestamp in milliseconds
};

#endif