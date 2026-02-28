#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>
#include <time.h>
#include "protocol.h"

volatile int running = 1;

void handle_signal(int sig) {
    (void)sig;
    running = 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return 1;
    }

    signal(SIGINT, handle_signal);

    int port = atoi(argv[1]);
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return 1;
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        close(server_fd);
        return 1;
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr.s_addr = INADDR_ANY
    };

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, 5) < 0) {
        perror("listen");
        close(server_fd);
        return 1;
    }

    printf("Simulator listening on port %d. Press Ctrl+C to stop.\n", port);

    while (running) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            if (running) {
                perror("accept");
            }
            continue;
        }

        printf("Client connected\n");

        packet_t pkt;
        uint8_t payload[20];

        // Heartbeat
        snprintf((char*)payload, sizeof(payload), "heartbeat");
        fill_packet(&pkt, MSG_HEARTBEAT, payload, (uint16_t)(strlen((char*)payload) + 1));
        if (send(client_fd, &pkt, sizeof(pkt), 0) < 0) {
            perror("send");
        }
        sleep(1);

        // Status
        snprintf((char*)payload, sizeof(payload), "status: ok");
        fill_packet(&pkt, MSG_STATUS, payload, (uint16_t)(strlen((char*)payload) + 1));
        if (send(client_fd, &pkt, sizeof(pkt), 0) < 0) {
            perror("send");
        }
        sleep(1);

        // Alarm
        snprintf((char*)payload, sizeof(payload), "alarm: fire");
        fill_packet(&pkt, MSG_ALARM, payload, (uint16_t)(strlen((char*)payload) + 1));
        if (send(client_fd, &pkt, sizeof(pkt), 0) < 0) {
            perror("send");
        }
        sleep(1);

        close(client_fd);
        printf("Client disconnected\n");
    }

    close(server_fd);
    printf("Simulator stopped.\n");
    return 0;
}
