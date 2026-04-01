#include "server.h"
// #include "handle_request.h"

int main () {
    int server_sock;
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, IP, &server_addr.sin_addr);

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("Socket error");
        close(server_sock);
        return -1;
    }

    int opt = 1;
    if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        close(server_sock);
        return -1;
    }
    

    if (bind(server_sock, (const struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("bind error");
        close(server_sock);
        return -1;
    }

    if (listen(server_sock, MAX_CONNECTIONS) < 0) {
        perror("failed to listen");
        close(server_sock);
        return -1;
    }

    while (true) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_size = sizeof(client_addr);
        memset(&client_addr, 0, client_addr_size);
        int client_socket = accept(server_sock, (struct sockaddr *) &client_addr , &client_addr_size);
        if (client_socket < 0) {
            perror("failed to accept client connection");
            continue;
        }

        Request_Packet *req_packet = NULL;
        int received_req = recv_request(client_socket, &req_packet);

        if (received_req < 0 || req_packet == NULL) {
            fprintf(stderr, "Failed to receive request\n");
            continue;
        }

        /* TODO: handle request */
        handle_request(req_packet); // TODO: handle request and response
        
        int sent_res_head = send_res_header(client_socket, res, res->msg_len);
        if (sent_res_head < 0) {
            fprintf(stderr, "Failed to send response\n");
            continue;
        }
        
        fprintf(stdout, "done sending in the server\n");

        // free(res);
        // free(req_packet);


        /* BUG: check if the operation is reciving in the form of sting or int i think it's int and it it's int then we are going to have trouble */

        
    }
    
    return 0;
}