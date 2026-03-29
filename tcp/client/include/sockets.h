#ifndef SOCKETS_H
#define SOCKETS_H

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "utils.h"

#define IP "127.0.0.1" /* 142.250.206.4 */
#define PORT 8080 /* 80 */

// bool send_data(const struct Request_Packet *packet);

#endif