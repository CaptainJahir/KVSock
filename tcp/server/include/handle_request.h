#ifndef HANDLE_REQUEST_H
#define HANDLE_REQUEST_H


#include "crud.h"
#include "arithmetic.h"
#include "utils.h"


void handle_request(int socket, Request_Packet *req, Hash_Table *table);


#endif