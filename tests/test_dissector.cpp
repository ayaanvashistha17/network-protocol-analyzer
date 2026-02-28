#include "test_runner.cpp"
#include "../dissector/dissector_core.h"
#include <cstring>
#include <arpa/inet.h>

extern "C" {
#include "../simulator/protocol.h"
}

TEST(test_parse_valid) {
    packet_t pkt;
    uint8_t payload[] = "hello";
    fill_packet(&pkt, MSG_STATUS, payload, 6);

    PacketDissector dissector;
    PacketDissector::PacketInfo info;
    bool ok = dissector.parse(reinterpret_cast<uint8_t*>(&pkt), sizeof(pkt), info);

    ASSERT(ok);
    ASSERT(info.magic == PROTOCOL_MAGIC);
    ASSERT(info.type == MSG_STATUS);
    ASSERT(info.type_string == "STATUS");
    ASSERT(info.length == 6);
    ASSERT(info.data.size() == 6);
    ASSERT(memcmp(info.data.data(), "hello", 6) == 0);
    ASSERT(info.crc_valid);
}

TEST(test_parse_invalid_magic) {
    packet_t pkt;
    uint8_t payload[] = "hello";
    fill_packet(&pkt, MSG_STATUS, payload, 6);
    pkt.magic = htons(0x1234); // wrong magic in packet byte order

    PacketDissector dissector;
    PacketDissector::PacketInfo info;
    bool ok = dissector.parse(reinterpret_cast<uint8_t*>(&pkt), sizeof(pkt), info);
    ASSERT(!ok);
}

int main() {
    add_test("test_parse_valid", test_parse_valid);
    add_test("test_parse_invalid_magic", test_parse_invalid_magic);
    return run_tests();
}
