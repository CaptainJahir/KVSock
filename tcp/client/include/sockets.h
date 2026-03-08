#ifndef SOCKETS_H
#define SOCKETS_H

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>

#define IP "127.0.0.1" /* 142.250.206.4 */
#define PORT 8080 /* 80 */

struct Request_Packet {
    char operation[16];
    char var_one_name[64];
    char var_one_data[256];
    char var_two_name[64]; // used in case of arithmetic operation
    // char var_two_data[256]; // no use as of now so commented this part
};

bool send_data(const struct Request_Packet *packet);

#endif