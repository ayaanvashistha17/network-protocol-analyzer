#ifndef DISSECTOR_CORE_H
#define DISSECTOR_CORE_H

#include <cstdint>
#include <string>
#include <vector>

class PacketDissector {
public:
    struct PacketInfo {
        uint16_t magic;
        uint8_t version;
        uint8_t type;
        uint16_t length;
        uint32_t timestamp;
        std::vector<uint8_t> data;
        uint16_t crc;
        bool crc_valid;
        std::string type_string;
    };

    // Parse raw packet bytes. Returns true if valid (magic, crc), false otherwise.
    bool parse(const uint8_t* raw, size_t len, PacketInfo& info);

    // Get a human-readable string representation.
    std::string to_string(const PacketInfo& info);

private:
    uint16_t crc16(const uint8_t* data, size_t len);
};

#endif
