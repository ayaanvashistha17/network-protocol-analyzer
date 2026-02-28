#include "protocol.h"
#include <string.h>
#include <time.h>
#include <arpa/inet.h>

uint16_t crc16(const uint8_t *data, size_t len) {
    uint16_t crc = 0xFFFF;
    for (size_t i = 0; i < len; i++) {
        crc ^= data[i] << 8;
        for (int j = 0; j < 8; j++) {
            if (crc & 0x8000)
                crc = (crc << 1) ^ 0x1021;
            else
                crc <<= 1;
        }
    }
    return crc;
}

void fill_packet(packet_t *pkt, uint8_t type, const uint8_t *payload, uint16_t len) {
    memset(pkt, 0, sizeof(*pkt));

    pkt->magic = htons(PROTOCOL_MAGIC);
    pkt->version = 1;
    pkt->type = type;
    pkt->length = htons(len);
    pkt->timestamp = htonl((uint32_t)time(NULL));

    if (payload && len > 0) {
        memcpy(pkt->data, payload, len);
    }

    // CRC is calculated over all bytes before the crc field
    uint16_t crc = crc16((const uint8_t*)pkt, sizeof(packet_t) - sizeof(pkt->crc));
    pkt->crc = htons(crc);
}
