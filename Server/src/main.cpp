#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("192.168.1.130");
    address.sin_port = htons(8080);
    bind(fd, (struct sockaddr*)&address, sizeof(address));

    listen(fd, 0);

    printf("Server is Running!\n");
    int addrlen = sizeof(address);
    int new_socket = accept(fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);

    if (new_socket < 0)
    {
        printf("Failed to accept connection.\n");
        return(-1);
    }
    
    while (true)
    {
        printf("Connection accepted.\n");

        char rdbuf[4096] = {0};
        read(new_socket, rdbuf, 4096);
        printf("Client Message: %s\n", rdbuf);

        const char* response = "Hello From Server!";
        write(new_socket, response, strlen(response));
    }
    close(new_socket);

    return 0;
}
