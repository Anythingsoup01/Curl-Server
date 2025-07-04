#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

const int MAX = 4096;

int fd;
struct sockaddr_in addr;

int main(int argc, char* argv[])
{
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        printf("Failed to create socket!");
        return -1;
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.1.130");
    addr.sin_port = htons(8080);
    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        printf("Failed to bind socket!");
        return -1;
    }

    listen(fd, 0);

    printf("Server is Running!\n");
    
    while (true)
    {
        int addrlen = sizeof(addr);
        int new_socket = accept(fd, (struct sockaddr*)&addr, (socklen_t*)&addrlen);

        if (new_socket < 0)
        {
            printf("Failed to accept connection.\n");
            return(-1);
        }

        char buff[MAX];
        while (true)
        {
            bzero(buff, sizeof(buff));
            read(new_socket, buff, sizeof(buff));
            if (strncmp(buff, "exit", 4))
            {
                break;
            }
            printf("client: %s", buff);
            bzero(buff, sizeof(buff));
            buff[0] = 'E'; buff[1] = '\n';
            write(new_socket, buff, sizeof(buff));

        }

        close(new_socket);

    }

    return 0;
}
