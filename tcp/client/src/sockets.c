#include "sockets.h"

/* -> additional function input:  , char *output */

bool send_data(const struct Request_Packet *packet) {
    if (packet == NULL) return false;
    errno = 0;
    int s;
    struct sockaddr_in addr;
    char recv_buff[8192];

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

    const char *ptr = (const char *)packet;
    size_t remaining = sizeof(*packet);
    while (remaining > 0) {
        ssize_t sent = send(s, packet, remaining, MSG_NOSIGNAL);
        if (sent < 0) {
            if (errno == EINTR) continue;
            perror("failed to send data");
            close(s);
            return false;
        }
        ptr += sent;
        remaining -= sent;
    }
    

    /*
        Your recv call reads only once. If the server’s response is larger than your buffer or arrives in
        multiple chunks, you might get only part of it. For a simple protocol where responses are small, this
        is often acceptable. But if you want to be robust, you’d need a loop until you’ve received 
        the expected amount or a timeout occurs.
    */ 

    int bytes = recv(s, recv_buff, sizeof(recv_buff)-1, 0);
    if (bytes > 0) {
        recv_buff[bytes] = '\0';
        printf("%s\n", recv_buff);
    } else if (bytes == 0) {
        printf("Server closed the connection.\n");
    } else {
        perror("recv error");
        close(s);
        return false;
    }
    
    close(s);
    return true;
}