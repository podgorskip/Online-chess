#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{

    int status, sock;
    struct sockaddr_in addr;
    char buffer[1000];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        printf("[-]Socket error.\n");
        return 0;
    }
    else
    {
        printf("[+]TCP server socket created.\n");
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(8000);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    status = connect(sock, (struct sockaddr *)&addr, sizeof addr);
    if (status < 0)
    {
        printf("[-]Connect error.\n");
        return 0;
    }
    else
    {
        printf("Connected to the server.\n");
    }

    while (1)
    {
        status = recv(sock, buffer, sizeof(buffer), 0); // gets board after computer's move
        buffer[status] = '\0';
        printf("%s\n", buffer);
        bzero(buffer, sizeof(buffer));

        status = recv(sock, buffer, sizeof(buffer), 0); // gets order to give coordinates
        if(status<1) {
            printf("You lose.");
            close(sock);
            exit(0);
        }
        buffer[status] = '\0';
        printf("%s", buffer);
        bzero(buffer, sizeof(buffer));

        scanf("%s", &buffer[0]); // gets coordinates from user
        scanf("%s", &buffer[1]);
        status = send(sock, buffer, strlen(buffer), 0);
        bzero(buffer, sizeof(buffer));

        status = recv(sock, buffer, sizeof(buffer), 0); // prints order to give coordinates
        buffer[status - 1] = '\0';
        printf("%s", buffer);
        bzero(buffer, sizeof(buffer));

        scanf("%s", &buffer[0]); // gets coordinates from user
        scanf("%s", &buffer[1]);
        status = send(sock, buffer, strlen(buffer), 0);
        bzero(buffer, sizeof(buffer));

        status = recv(sock, buffer, sizeof(buffer), 0); // prints board after user's move
        buffer[status] = '\0';
        printf("%s\n", buffer);
        bzero(buffer, sizeof(buffer));

    }

    return 0;
}
