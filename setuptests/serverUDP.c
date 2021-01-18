#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../setup.h"

#define MAXBUF 100
#define PORT "30000"

int main()
{
    int rv;
    SOCK_INFO *info;
    struct addrinfo hints;
    char msg[MAXBUF];

    memset(msg, 0, MAXBUF);
    
    if ( (info = malloc(sizeof(SOCK_INFO))) == NULL )
    {
        printf("server: malloc\n");
        exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    if( (info->sockfd = set_passive_udp(&hints, PORT)) == -1 )
        exit(EXIT_FAILURE);
    
    info->addr_len = sizeof(*(info->addr));
    while(1)
    {
        rv = recv_udp(msg, MAXBUF, info);
        if ( rv == -1 )
            exit(EXIT_FAILURE);
        printf("%s", msg);
        memset(msg, 0, MAXBUF);
    }
    return 0;
}
