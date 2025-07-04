#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()

const int MAX = 4096;

int fd;
struct sockaddr_in addr;

bool running = true;

int main(int argc, char* argv[])
{
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        printf("Error: Failed to create Socket!");
        exit(-1);
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.1.130");
    addr.sin_port = htons(8080);
    
    if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) != 0)
    {
        printf("Failed to connect to server!\n");
        exit(-1);
    }

    char buff[MAX];
    int n;
    for (;;)
    {
        bzero(buff, sizeof(buff));
        printf("##: ");

        n = 0;

        while ((buff[n++] = getchar()) != '\n');


        write(fd, buff, sizeof(buff));


        if (strncmp(buff, "exit", 4) == 0)
            break;

        bzero(buff, sizeof(buff));
        read(fd, buff, sizeof(buff));
        printf("%s\n", buff);

    }

    close(fd);
    /*
    const char* msg = "Hello From Client!";
    write(fd, msg, strlen(msg));

    char response[1024] = {0};
    read(fd, response, 1024);
    printf("Server says: %s\n", response);
    */
    return 0;
}
