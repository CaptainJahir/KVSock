#ifndef SOCKETS_H
#define SOCKETS_H

#include <netinet/in.h>
#include <unistd.h>

#include "utils.h"
#include "protocols.h"

#define IP "127.0.0.1" /* 142.250.206.4 */
#define PORT 8080 /* 80 */

bool execute_request(const Request_Packet *packet, size_t dynamic_arr_size, char *opr_name);

#endif