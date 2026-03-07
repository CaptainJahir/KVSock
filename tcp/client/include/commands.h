#ifndef COMMANDS_H
#define COMMANDS_H

#include "sockets.h"
#include "utils.h"

bool handle_cmd_set(char *data, struct Request_Packet *packet);
bool handle_cmd_get(char *data, struct Request_Packet *packet);
bool handle_cmd_list(char *data, struct Request_Packet *packet);
bool handle_cmd_arith(char *data, struct Request_Packet *packet);


#endif