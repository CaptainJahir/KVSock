#ifndef PROTOCOLS_H
#define PROTOCOLS_H

#include "utils.h"
#include <stddef.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define FAILURE -1
#define SUCCESS 0

int send_request(int s, Request_Packet *packet, size_t dynamic_arr_size);
int send_res_header (int s, Response_Header *head, size_t dynamic_arr_size);
int send_res_packet (int s, Response_Packet *packet, size_t dynamic_arr_size);
int recv_request(int s, Request_Packet **packet);
int recv_res_header (int s, Response_Header **head);
int recv_res_packet (int s, Response_Packet **res_pack);
Response_Header* construct_res_header(uint16_t item_count, int8_t success, uint8_t msg_len, char* msg);
void send_header_response(int socket, uint16_t item_count, char* message, int flags);


#endif