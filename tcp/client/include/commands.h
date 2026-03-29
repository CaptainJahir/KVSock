#ifndef COMMANDS_H
#define COMMANDS_H

#include "utils.h"
#include "sockets.h"

bool handle_cmd_set(Request_Packet *packet, char *tokens[], size_t token_sizes[]);
bool handle_cmd_get(Request_Packet *packet, char *tokens[], size_t token_sizes[]);
bool handle_cmd_list(Request_Packet *packet, char *tokens[], size_t token_sizes[]);
bool handle_cmd_arith(Request_Packet *packet, char *tokens[], size_t token_sizes[]);


#endif