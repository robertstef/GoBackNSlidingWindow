#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <setup.h>

#define MAXBUF 100

int main(int argc, char *argv[])
{
    int rv;
    SOCK_INFO *info;
    struct addrinfo hints;
    char msg[MAXBUF];

    if ( argc != 3 )
    {
        printf("usage: uclient port hostname\n");
        exit(EXIT_FAILURE);
    }

    // setup hints
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ( (info = set_active_udp(&hints, argv[1], argv[2])) == NULL )
        exit(EXIT_FAILURE);

    memset(msg, 0, MAXBUF);
    while(1)
    {
        printf(">> ");
        fgets(msg, MAXBUF, stdin);
        rv = send_udp(msg, MAXBUF, info);
        if ( rv == -1 )
            exit(EXIT_FAILURE);
    }

    return 0;
}
