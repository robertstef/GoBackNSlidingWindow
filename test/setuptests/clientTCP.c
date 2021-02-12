#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../setup.h"

int main(int argc, char *argv[])
{
    int rv;
    int sockfd;
    struct addrinfo hints;
    char msg[100];

    if ( argc != 3 )
    {
        fprintf(stderr, "usage: tclient hostname port\n");
        exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;


    if ( (sockfd = set_active_tcp(&hints, argv[2], argv[1])) == -1 )
        exit(EXIT_FAILURE);

    while(1)
    {
        fprintf(stdout, ">> ");
        fgets(msg, 100, stdin);

        rv = send_tcp(msg, sockfd, 100);
        if ( rv == -1 )
            exit(EXIT_FAILURE);
    }

    return 0;
}
