#include "sockets.h"


bool execute_request(Request_Packet *packet, size_t dynamic_arr_size, char *opr_name) {
    /*
        IMPORTANT: 
            `char *opr_name` should be of `char opr_name[] = "somthing";` before passing into the function
            Simply: pass writable memory location not readonly memory location
            else segmentation fault will arise
    */

    if (packet == NULL) return false;
    errno = 0;
    int s;
    struct sockaddr_in addr;
    Response_Header *res_head;
    Response_Packet *res_packet;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, IP, &addr.sin_addr) <= 0) {
        fprintf(stderr, "Invalid IP address\n");
        return false;
    }

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        perror("socket error");
        return false;
    }
    
    if (connect(s, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        perror("Failed to connect to the server");
        close(s);
        return false;
    }

    int sent_req = send_request(s, packet, dynamic_arr_size);
    if (sent_req < 0) {
        close(s);
        return false;
    }

    Response_Header *head = NULL;
    int received_header =  recv_res_header(s, &head);
    if (received_header < 0 || head == NULL) {
        fprintf(stderr, "Failed to receive data \n");
        close(s);
        return false;
    }

    fprintf(stdout, "%s\n", head->msg);

    uint16_t item_count = head->item_count;
    if (head->success < 0) {
        char *msg = head->msg;
        fprintf(stderr, "%s", msg);
        return false;
    }
    
    free(head);
    while (item_count > 0) {
        Response_Packet *res_packet = NULL;
        int received_response = recv_res_packet(s, &res_packet);

        if (received_response < 0 || res_packet == NULL) {
            fprintf(stderr, "Failed to get item\n");
            item_count--;
            continue;
        }
        
        char *key = res_packet->data;
        char *val = res_packet->data + res_packet->key_len;
        
        fprintf(stdout, "[%s] Key %s = %s\n", to_upper_case(opr_name), key, val);
        item_count--;
        free(res_packet);
    }
    
    close(s);
    return true;
}