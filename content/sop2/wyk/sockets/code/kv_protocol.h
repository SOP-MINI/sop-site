#ifndef KV_PROTOCOL_H
#define KV_PROTOCOL_H

#include <stdint.h>

#define OP_READ   1
#define OP_WRITE  2
#define OP_DELETE 3
#define OP_FIRE   4

#define STATUS_OK         0
#define STATUS_NOT_FOUND  1
#define STATUS_ERROR      2

/* Client -> Server Request Header (9 bytes) */
struct request_hdr {
    uint8_t  op;        // OP_READ, OP_WRITE, OP_DELETE
    uint32_t key;       // Integer key
    uint32_t val_len;   // Length of the string payload (only for OP_WRITE)
} __attribute__((packed));

/* Server -> Client Response Header (5 bytes) */
struct response_hdr {
    uint8_t  status;    // STATUS_OK, etc.
    uint32_t val_len;   // Length of the returned string (only for OP_READ)
} __attribute__((packed));

#endif