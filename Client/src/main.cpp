#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()

int main(int argc, char* argv[])
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.1.130");
    addr.sin_port = htons(8080);

    if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) != 0)
    {
        printf("connection failed!");
        exit(0);
    }

    const char* msg = "Hello From Client!";
    write(fd, msg, strlen(msg));

    char response[1024] = {0};
    read(fd, response, 1024);
    printf("Server says: %s\n", response);
    return 0;
}
