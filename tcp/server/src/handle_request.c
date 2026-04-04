#include "handle_request.h"

void handle_request(int socket, Request_Packet *req, Hash_Table *table) {
    if (req->operation == SET_STR || req->operation == SET_NUM || req->operation == UPDATE) {
        handle_cmd_set(socket, req, table);
    } else if (req->operation == GET || req->operation == DELETE) {
        handle_cmd_get(socket, req, table);
    } else if (req->operation == LIST || req->operation == DEL_ALL) {
        handle_cmd_list(socket, req, table);
    } else if (req->operation == ADD || req->operation == SUB || req->operation == MUL || req->operation == DIV) {
        handle_cmd_arith(socket, req, table);
    } else {
        fprintf(stderr, "Invalid operation selected\n");
    }
}