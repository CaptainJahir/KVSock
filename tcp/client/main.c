#include "main.h"

int main (int argc, char *argv[])
{
    int s;
    struct sockaddr_in addr;
    char buf[512] = "GET / HTTP/1.1\r\n"
                    "Host: www.google.com\r\n"
                    "Connection: close\r\n"
                    "\r\n";
    char recv_buff[8192];


    addr.sin_port = htons(PORT);
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, IP, &addr.sin_addr);

    s = socket(AF_INET, SOCK_STREAM, 0);

    if (connect(s, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Socket error");
        close(s);
        return -1;
    }

    write(s, buf, sizeof(buf));

    send(s, buf, sizeof(buf), 0);

    recv(s, recv_buff, sizeof(recv_buff), 0);

    printf("%s\n", recv_buff);

    close(s);
    return 0;
}