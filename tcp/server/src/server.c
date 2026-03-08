#include "server.h"

int main () {
    int server_sock;
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, IP, &server_addr.sin_addr);

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("Socket error");
        close(server_sock);
        return -1;
    }

    if (bind(server_sock, (const struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("bind error");
        close(server_sock);
        return -1;
    }

    if (listen(server_sock, MAX_CONNECTIONS) < 0) {
        perror("failed to listen");
        close(server_sock);
        return -1;
    }

    while (true) {
        struct sockaddr_in client_addr;
        struct Request_Packet req_packet;
        memset(&req_packet, 0, sizeof(req_packet));
        socklen_t client_addr_size = sizeof(client_addr);
        memset(&client_addr, 0, client_addr_size);
        int client_socket = accept(server_sock, (struct sockaddr *) &client_addr , &client_addr_size);
        if (client_socket < 0) {
            perror("failed to accept client connection");
            continue;
        }

        ssize_t total = 0;
        ssize_t bytes;
        while (total < sizeof(req_packet)) {
            bytes = recv(client_socket, (char *)&req_packet + total, sizeof(req_packet) - total, 0);
            if (bytes < 0) {
                perror("failed to recive data");
                break;
            } else if (bytes == 0) {
                fprintf(stderr, "No data recived or connection closed");
                break;
            }
            total += bytes;
        }
        
        /*
        
        if (total != sizeof(req_packet)) {
            fprintf(stderr, "Incomplete packet received\n");
        }

        */

        /* TODO: handle recv -> process -> send */
    }
    
    return 0;
    
    /*
        WORK FLOW:
            socket → bind → listen → loop( accept → receive → process → send )
    */
}