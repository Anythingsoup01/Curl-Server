#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

const int MAX = 4096;

int fd;
struct sockaddr_in addr;


const char* ParseInput(const char input[])
{
    if (strncmp(input, "exit", 4) == 0)
        return "Goodbye!";

    if (strncmp(input, "shutdown", 8) == 0)
        return "Shutting down server.";

    if (strncmp(input, "list", 4) == 0)
        return "Listing all entries";

    return "Command Not Found!";
}

int main(int argc, char* argv[])
{
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        printf("Failed to create socket!\n");
        return -1;
    }

    int enable = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    {
        printf("Failed to set socket option!");
        return -1;
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(8080);
    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        printf("Failed to bind socket!\n");
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

        printf("Client connected.\n");

        char buffin[MAX];
        char* buffout;
        while (true)
        {
            bzero(buffin, sizeof(buffin));
            read(new_socket, buffin, sizeof(buffin));
            buffout = (char*)ParseInput(buffin);
            printf("client: %s", buffin);
            printf("expected out: %s\n", buffout);
            write(new_socket, buffout, strlen(buffout));
            if (strncmp(buffin, "exit", 4) == 0 | strncmp(buffin, "shutdown", 8) == 0)
                break;
        }


        printf("Client not connected.\n");

        close(new_socket);

        if (strncmp(buffin, "shutdown", 8) == 0)
            break;

    }

    close(fd);

    return 0;
}
