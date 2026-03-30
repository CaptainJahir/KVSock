#include "protocols.h"

/*
TODO: set descriptors
         0  ->  success
        -1  -> failure
*/

int send_request(int s, Request_Packet *packet, size_t dynamic_arr_size) {
    const char *ptr = (const char *)packet;
    size_t remaining = offsetof(Request_Packet, data) + dynamic_arr_size;
    while (remaining > 0) {
        ssize_t sent = send(s, ptr, remaining, MSG_NOSIGNAL);
        if (sent < 0) {
            if (errno == EINTR) continue;
            perror("failed to send data");
            return -1;
        }

        if (sent == 0) {
            fprintf(stderr, "connection closed\n");
            return -1;
        }
        
        ptr += sent;
        remaining -= sent;
    }
    return 0;
}

int recv_request(int s, Request_Packet **packet) {
    Request_Packet *fixed_part = malloc(sizeof(Request_Packet));
    if (fixed_part == NULL) {
        return -1;
    }
    
    char *fixed_buff = (char *)fixed_part;
    size_t fixed_remaining = offsetof(Request_Packet, data);

    while (fixed_remaining > 0) {
        ssize_t n = recv(s, fixed_buff, fixed_remaining, 0);
        if (n <= 0) {
            free(fixed_part);
            return -1;
        }
        
        fixed_buff += n;
        fixed_remaining -= n;
    }
    
    fixed_part->var_one_data_len = ntohs(fixed_part->var_one_data_len);
    size_t remaining =  fixed_part->arith_store_var_len + 
                        fixed_part->var_one_name_len + 
                        fixed_part->var_one_data_len + 
                        fixed_part->var_two_name_len;

    Request_Packet *full = malloc(offsetof(Request_Packet, data) + remaining);

    if (full == NULL) {
        free(fixed_part);
        return -1;
    }

    memcpy(full, fixed_part, offsetof(Request_Packet, data));
    free(fixed_part);

    char *buff = full->data;

    while (remaining > 0) {
        ssize_t received = recv(s, buff, remaining, 0);

        if (received <= 0) {
            free(full);
            return -1;
        }

        buff += received;
        remaining -= received;
    }

    *packet = full;
    return 0;
    // TODO: free full after execution of the function
}

int send_res_header (int s, Response_Header *head, size_t dynamic_arr_size) {
    head->item_count = htons(head->item_count);
    char *buf = (char *)head;
    size_t remaining = offsetof(Response_Header, msg) + dynamic_arr_size;

    while (remaining > 0) {
        ssize_t sent = send(s, buf, remaining,  MSG_NOSIGNAL);
        if (sent < 0) {
            if (errno == EINTR) continue;
            perror("failed to send data");
            return -1;
        }

        if (sent == 0) {
            fprintf(stderr, "connection closed\n");
            return -1;
        }

        buf += sent;
        remaining -= sent;
    }
    
    return 0;
}

int recv_res_header (int s, Response_Header **head) {
    // TODO: free full after the use of it (beyond this function)
    Response_Header *fixed_part = calloc(1, sizeof(Response_Header));
    if (fixed_part == NULL) {
        return -1;
    }
    
    char *fixed_buf = (char *)fixed_part;
    size_t remaining = offsetof(Response_Header, msg);

    while (remaining > 0) {
        ssize_t recv_header = recv(s, fixed_buf, remaining, 0);

        if (recv_header < 0) {
            if (errno == EINTR) continue;
            perror("Failed to receive fixed part of header");
            free(fixed_part);
            return -1;
        }

        if (recv_header == 0) {
            fprintf(stderr, "Connection closed\n");
            free(fixed_part);
            return -1;
        }
        
        
        fixed_buf += recv_header;
        remaining -= recv_header;
    }

    fixed_part->item_count = ntohs(fixed_part->item_count);
    size_t msg_remaining = fixed_part->msg_len;
    if (msg_remaining == 0 || msg_remaining > 255) {
        fprintf(stderr, "Size of message array exceeded limit (8 bit int): %zu\n", msg_remaining);
        free(fixed_part);
        return -1;
    }
    
    
    Response_Header *full = malloc(offsetof(Response_Header, msg) + msg_remaining);
    if (full == NULL) {
        free(fixed_part);
        fprintf(stderr, "Failed to allocate memory for full header\n");
        return -1;
    }
    
    memcpy(full, fixed_part, offsetof(Response_Header, msg));
    
    free(fixed_part);

    char *msg_buf = full->msg;

    while (msg_remaining > 0) {
        ssize_t received = recv(s, msg_buf, msg_remaining, 0);

        if (received < 0) {
            if (errno == EINTR) continue;
            perror("Failed to receive dynamic array of header");
            free(full);
            return -1;
        }

        if (received == 0) {
            fprintf(stderr, "Connection closed\n");
            free(full);
            return -1;
        }
        

        msg_buf += received;
        msg_remaining -= received;
    }
    
    *head = full;
    return 0;
}

int send_res_packet (int s, Response_Packet *packet, size_t dynamic_arr_size) {
    packet->val_len = htons(packet->val_len);
    char *buf = (char *)packet;
    size_t remaining = offsetof(Response_Packet, data) + dynamic_arr_size;

    while (remaining > 0) {
        ssize_t sent = send(s, buf, remaining,  MSG_NOSIGNAL);
        if (sent < 0) {
            if (errno == EINTR) continue;
            perror("failed to send data");
            return -1;
        }

        if (sent == 0) {
            fprintf(stderr, "connection closed\n");
            return -1;
        }

        buf += sent;
        remaining -= sent;
    }
    
    return 0;
}

int recv_res_packet (int s, Response_Packet **res_pack) {
    // TODO: free full after the use of it (beyond this function)
    Response_Packet *fixed_part = malloc(sizeof(Response_Packet));
    if (fixed_part == NULL) {
        return -1;
    }
    
    char *fixed_buf = (char *)fixed_part;
    size_t remaining = offsetof(Response_Packet, data);

    while (remaining > 0) {
        ssize_t recv_packet = recv(s, fixed_buf, remaining, 0);

        if (recv_packet < 0) {
            if (errno == EINTR) continue;
            perror("Failed to receive fixed part of response");
            free(fixed_part);
            return -1;
        }

        if (recv_packet == 0) {
            fprintf(stderr, "Connection closed\n");
            free(fixed_part);
            return -1;
        }
        
        
        fixed_buf += recv_packet;
        remaining -= recv_packet;
    }

    fixed_part->val_len = ntohs(fixed_part->val_len);
    size_t msg_remaining = fixed_part->val_len + fixed_part->key_len;
    
    Response_Packet *full = malloc(offsetof(Response_Packet, data) + msg_remaining);
    if (full == NULL) {
        free(fixed_part);
        fprintf(stderr, "Failed to allocate memory for full response\n");
        return -1;
    }
    
    memcpy(full, fixed_part, offsetof(Response_Packet, data));
    
    free(fixed_part);

    char *msg_buf = full->data;

    while (msg_remaining > 0) {
        ssize_t received = recv(s, msg_buf, msg_remaining, 0);

        if (received < 0) {
            if (errno == EINTR) continue;
            perror("Failed to receive dynamic array of response");
            free(full);
            return -1;
        }

        if (received == 0) {
            fprintf(stderr, "Connection closed\n");
            free(full);
            return -1;
        }

        msg_buf += received;
        msg_remaining -= received;
    }
    
    *res_pack = full;
    return 0;
}
