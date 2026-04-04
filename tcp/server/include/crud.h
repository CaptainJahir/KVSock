#ifndef CRUD_H
#define CRUD_H

#include "server.h"

int handle_cmd_set(int socket, Request_Packet *req, Hash_Table *table);
int handle_cmd_get(int socket, Request_Packet *req, Hash_Table *table);
int handle_cmd_list(int socket, Request_Packet *req, Hash_Table *table);

#endif
