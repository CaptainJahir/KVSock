#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#include "hash_table.h"
#include "protocols.h"

int handle_cmd_arith(int socket, Request_Packet *req, Hash_Table *table);


#endif