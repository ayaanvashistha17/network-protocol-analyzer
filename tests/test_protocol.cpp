#include "test_runner.cpp"
#include <cstring>
#include <arpa/inet.h>

extern "C" {
#include "../simulator/protocol.h"
}

TEST(test_crc) {
    uint8_t data[] = {
        0xAA, 0x55, 0x01, 0x02, 0x00, 0x05, 0x00, 0x00, 0x00, 0x01,
        'h', 'e', 'l', 'l', 'o'
    };
    uint16_t crc = crc16(data, sizeof(data));
    ASSERT(crc != 0);
}

TEST(test_packet_fill) {
    packet_t pkt;
    uint8_t payload[] = "test";
    fill_packet(&pkt, MSG_ALARM, payload, 5);

    ASSERT(ntohs(pkt.magic) == PROTOCOL_MAGIC);
    ASSERT(pkt.type == MSG_ALARM);
    ASSERT(ntohs(pkt.length) == 5);
    ASSERT(memcmp(pkt.data, "test", 5) == 0);

    uint16_t calc_crc = crc16((uint8_t*)&pkt, offsetof(packet_t, crc));
    ASSERT(calc_crc == ntohs(pkt.crc));
}

int main() {
    add_test("test_crc", test_crc);
    add_test("test_packet_fill", test_packet_fill);
    return run_tests();
}
