#ifndef SERVER_H
#define SERVER_H

#include "utils.h"
#include "protocols.h"
#include "hash_table.h"
#include "handle_request.h"

#include <netinet/in.h>
#include <unistd.h>
       

#define PORT 8080
#define IP "127.0.0.1"
#define MAX_CONNECTIONS 10
#define MAX_HASH_TABLE_SIZE 256



#endif