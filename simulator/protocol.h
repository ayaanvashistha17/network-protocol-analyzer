#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <stddef.h>

#define PROTOCOL_MAGIC 0xAA55
#define MAX_PAYLOAD 64

typedef enum {
    MSG_HEARTBEAT = 0x01,
    MSG_ALARM     = 0x02,
    MSG_STATUS    = 0x03,
    MSG_CONFIG    = 0x04
} message_type_t;

typedef struct {
    uint16_t magic;        // 0xAA55
    uint8_t  version;      // Protocol version
    uint8_t  type;         // message_type_t
    uint16_t length;       // Payload length
    uint32_t timestamp;    // seconds since boot
    uint8_t  data[MAX_PAYLOAD];
    uint16_t crc;          // CRC-16 of header+payload (excluding crc field)
} __attribute__((packed)) packet_t;

// CRC-16-CCITT (poly 0x1021) calculation
uint16_t crc16(const uint8_t *data, size_t len);

// Helper to fill a packet with given type and payload
void fill_packet(packet_t *pkt, uint8_t type, const uint8_t *payload, uint16_t len);

#endif
