#ifndef PROTOCOLS_H
#define PROTOCOLS_H

#include "utils.h"
#include <stddef.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/types.h>
#include <arpa/inet.h>

int send_request(int s, const Request_Packet *packet, size_t dynamic_arr_size);
int send_res_header (int s, Response_Header *head, size_t dynamic_arr_size);
int send_res_packet (int s, Response_Packet *packet, size_t dynamic_arr_size);
int recv_request(int s, Request_Packet **packet);
int recv_res_header (int s, Response_Header **head);
int recv_res_packet (int s, Response_Packet **res_pack);


#endif