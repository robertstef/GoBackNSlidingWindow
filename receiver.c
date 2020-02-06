/*
 * Robert Stefanyshin
 * ras867
 * 11137532
 * CMPT434
 * A2
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "setup.h"
#include "packet.h"

#define PORT "30000"

int main()
{
    int rv;
    SOCK_INFO *info;
    struct addrinfo hints;
    char msg[MAXBUF];
    PKT *pkt;

    // initialize variables
    memset(msg, 0, MAXBUF);
    
    if ( (info = malloc(sizeof(SOCK_INFO))) == NULL )
    {
        printf("server: malloc\n");
        exit(EXIT_FAILURE);
    }

    if ( (pkt = malloc(PKTSZ)) == NULL )
    {
        printf("server: malloc\n");
        exit(EXIT_FAILURE);
    }

    // create socket
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    if( (info->sockfd = set_passive_udp(&hints, PORT)) == -1 )
        exit(EXIT_FAILURE);
    
    info->addr_len = sizeof(*(info->addr));

    // main receive loop
    while(1)
    {
        rv = recv_udp(pkt, PKTSZ, info);
        if ( rv == -1 )
            exit(EXIT_FAILURE);
        printf("seqnum: %d\n", pkt->seqnum);
        printf("msg: %s\n", pkt->msg);
        memset(pkt, 0, PKTSZ);
    }
    return 0;
}
