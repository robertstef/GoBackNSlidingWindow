/*
 * Robert Stefanyshin
 * ras867
 * 11137532
 * CMPT434
 * A2
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "setup.h"
#include "packet.h"
#include "userinput.h"

#define PORT "35000"

int main(int argc, char *argv[])
{
    int rv;
    SOCK_INFO *rec_info;
    struct addrinfo hints;
    PKT *pkt;

    // check command line input
    if ( (rv = checkinput(argc, argv)) == -1 )
    {
        fprintf(stdout, "usage: IP port maxwin timeout\n");
        exit(EXIT_FAILURE);
    }
    
    // create packet
    if ( (pkt = malloc(PKTSZ)) == NULL )
        exit(EXIT_FAILURE);

    memset(pkt, 0, PKTSZ);

    // setup listening socket
    if ( (rec_info = malloc(sizeof(SOCK_INFO))) == NULL )
    {
        printf("server: malloc\n");
        exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    if( (rec_info->sockfd = set_passive_udp(&hints, PORT)) == -1 )
        exit(EXIT_FAILURE);
    
    rec_info->addr_len = sizeof(rec_info->addr);

    while(1)
    {
        rv = recv_udp(pkt, MAXBUF, rec_info);
        if ( rv == -1 )
            exit(EXIT_FAILURE);
        printf("%s", pkt->msg);
        memset(pkt, 0, PKTSZ);
    }

    return 0;
}
