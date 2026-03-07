#include "server.h"

int main (int argc, char *argv[]) {
    int s;
    struct sockaddr_in server_sockaddr;
    struct sockaddr_in client_sockaddr;
    memset(&server_sockaddr, 0, sizeof(server_sockaddr));
    memset(&client_sockaddr, 0, sizeof(client_sockaddr));
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(PORT);

    char data[8192];

    if (inet_pton(AF_INET, IP, &server_sockaddr.sin_addr) <= 0)
    {
        perror("inet_pton failed");
        return -1;
    }
    

    s = socket(AF_INET, SOCK_STREAM, 0);

    if (s < 0)
    {
        perror("Socket error");
        close(s);
        return -1;
    }
    
    if ((bind(s, (struct sockaddr *) &server_sockaddr, sizeof(server_sockaddr))) < 0)
    {
        perror("Failed to bind socket");
        close(s);
        return -1;
    }

    int listening_respo = listen(s, 5);

    if (listening_respo == 0)
    {
        printf("The server is listening on %s:%d\n", IP, PORT);
    }else {
        perror("Failed to listen");
    }

    socklen_t client_size = sizeof(client_sockaddr);

    int client_sock = accept(s, (struct sockaddr *)&client_sockaddr, &client_size);
    
    if (client_sock < 0)
    {
        printf("Client got connected\n");
    }

    recv(client_sock, data, sizeof(data), 0);
    printf("%s\n", data);
    send(client_sock, data, strlen(data), 0);
    
    printf("OK!\n");
    close(s);
    return 0;
}