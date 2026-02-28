#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "dissector_core.h"
#include "../simulator/protocol.h"  

void print_usage(const char* prog) {
    std::cerr << "Usage:\n";
    std::cerr << "  " << prog << " capture <host> <port> <output_file>\n";
    std::cerr << "  " << prog << " analyze <input_file>\n";
}

bool capture_packets(const std::string& host, int port, const std::string& outfile) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return false;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, host.c_str(), &addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sock);
        return false;
    }

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect");
        close(sock);
        return false;
    }

    std::ofstream ofs(outfile, std::ios::binary);
    if (!ofs) {
        std::cerr << "Failed to open output file: " << outfile << std::endl;
        close(sock);
        return false;
    }

    uint8_t buffer[sizeof(packet_t)];
    ssize_t received;
    while ((received = recv(sock, buffer, sizeof(buffer), 0)) > 0) {
        ofs.write(reinterpret_cast<char*>(buffer), received);
    }

    ofs.close();
    close(sock);
    return true;
}

bool analyze_file(const std::string& infile) {
    std::ifstream ifs(infile, std::ios::binary);
    if (!ifs) {
        std::cerr << "Failed to open input file: " << infile << std::endl;
        return false;
    }

    PacketDissector dissector;
    PacketDissector::PacketInfo info;
    uint8_t buffer[sizeof(packet_t)];

    while (ifs.read(reinterpret_cast<char*>(buffer), sizeof(buffer))) {
        if (dissector.parse(buffer, sizeof(buffer), info)) {
            std::cout << dissector.to_string(info);
        } else {
            std::cerr << "Invalid packet found, skipping.\n";
        }
    }
    return true;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    std::string mode = argv[1];
    if (mode == "capture" && argc == 5) {
        std::string host = argv[2];
        int port = std::stoi(argv[3]);
        std::string outfile = argv[4];
        if (!capture_packets(host, port, outfile)) {
            return 1;
        }
        std::cout << "Capture complete. Packets saved to " << outfile << std::endl;
    }
    else if (mode == "analyze" && argc == 3) {
        std::string infile = argv[2];
        if (!analyze_file(infile)) {
            return 1;
        }
    }
    else {
        print_usage(argv[0]);
        return 1;
    }
    return 0;
}
