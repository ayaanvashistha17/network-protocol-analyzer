
#include "dissector_core.h"
#include "../simulator/protocol.h"
#include <cstring>
#include <iomanip>
#include <sstream>
#include <arpa/inet.h>

static const char* type_to_string(uint8_t type) {
    switch(type) {
        case MSG_HEARTBEAT: return "HEARTBEAT";
        case MSG_ALARM: return "ALARM";
        case MSG_STATUS: return "STATUS";
        case MSG_CONFIG: return "CONFIG";
        default: return "UNKNOWN";
    }
}

bool PacketDissector::parse(const uint8_t* raw, size_t len, PacketInfo& info) {
    if (len < sizeof(packet_t)) return false;

    uint16_t magic;
    memcpy(&magic, raw, sizeof(magic));
    info.magic = ntohs(magic);
    if (info.magic != PROTOCOL_MAGIC) return false;

    info.version = raw[2];
    info.type = raw[3];

    uint16_t len_be;
    memcpy(&len_be, raw + 4, sizeof(len_be));
    info.length = ntohs(len_be);
    if (info.length > MAX_PAYLOAD) return false;

    uint32_t ts_be;
    memcpy(&ts_be, raw + 6, sizeof(ts_be));
    info.timestamp = ntohl(ts_be);

    info.data.assign(raw + 10, raw + 10 + info.length);

    uint16_t crc_be;
    memcpy(&crc_be, raw + offsetof(packet_t, crc), sizeof(crc_be));
    info.crc = ntohs(crc_be);

    uint16_t calc_crc = crc16(raw, offsetof(packet_t, crc));
    info.crc_valid = (calc_crc == info.crc);

    info.type_string = type_to_string(info.type);
    return true;
}

std::string PacketDissector::to_string(const PacketInfo& info) {
    std::ostringstream oss;
    oss << "Packet:\n";
    oss << "  Magic: 0x" << std::hex << info.magic << std::dec << "\n";
    oss << "  Version: " << (int)info.version << "\n";
    oss << "  Type: " << info.type_string << " (" << (int)info.type << ")\n";
    oss << "  Length: " << info.length << "\n";
    oss << "  Timestamp: " << info.timestamp << "\n";
    oss << "  Data: ";
    for (uint8_t b : info.data) {
        if (isprint(b)) oss << (char)b;
        else oss << "\\x" << std::hex << std::setw(2) << std::setfill('0') << (int)b << std::dec;
    }
    oss << "\n";
    oss << "  CRC: 0x" << std::hex << info.crc << std::dec;
    oss << " (" << (info.crc_valid ? "valid" : "INVALID") << ")\n";
    return oss.str();
}
uint16_t PacketDissector::crc16(const uint8_t* data, size_t len) {
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
