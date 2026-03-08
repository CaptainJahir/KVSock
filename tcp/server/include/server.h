#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
       

#define PORT 8080
#define IP "127.0.0.1"
#define MAX_CONNECTIONS 10

struct Request_Packet {
    char operation[16];
    char var_one_name[64];
    char var_one_data[256];
    char var_two_name[64]; // used in case of arithmetic operation
    // char var_two_data[256]; // no use as of now so commented this part
};


#endif