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
    addr.sin_addr.s_addr = inet_addr("192.168.1.125");
    addr.sin_port = htons(8080);
    
    if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) != 0)
    {
        printf("Failed to connect to server!\n");
        exit(-1);
    }

    char buffin[MAX];
    char buffout[MAX];
    int n;
    for (;;)
    {
        bzero(buffin, sizeof(buffin));
        printf("##: ");
        n = 0;

        while ((buffin[n++] = getchar()) != '\n');
        write(fd, buffin, sizeof(buffin));

        bzero(buffout, sizeof(buffout));
        read(fd, buffout, sizeof(buffout));
        if (strlen(buffout) > 0)
            printf("%s\n", buffout);

        if (strncmp(buffin, "exit", 4) == 0 | strncmp(buffin, "shutdown", 8) == 0)
            break;
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
