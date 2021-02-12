#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <setup.h>
#include <packet.h>
#include "receiverfcns.h"

#define PORT "30000"

int main(int argc, char *argv[])
{
    int rv;
    SOCK_INFO *info;
    struct addrinfo hints;
    char msg[MAXBUF];
    PKT *pkt;
    int p_ack;

    // check command line input
    if ( argc != 2 )
    {
        fprintf(stdout, "usage: receiver p_ack\n");
        exit(EXIT_FAILURE);
    }

    if ( (p_ack = atoi(argv[1])) < 0 )
    {
        fprintf(stderr, "p_ack must be an integer in range 0-10\n");
        fprintf(stdout, "usage: receiver p_ack\n");
        exit(EXIT_FAILURE);
    }

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

    // init receiver
    rec_init(p_ack);
    
    info->addr_len = sizeof(info->addr);

    // main receive loop
    while(1)
    {
        // get packet from sender
        if ( (rv = recv_udp(pkt, PKTSZ, info)) == -1 )
            exit(EXIT_FAILURE);
        
        // parse packet
        rv = parse_pkt(pkt);

        switch(rv)
        {
            case(NEXT):
                if ( (rv = rec_next(pkt, info)) == -1 )
                    exit(EXIT_FAILURE);
                break;
            case(OUTOFORDER):
                fprintf(stdout, "MESSAGE OUT OF ORDER\nSeq num:" 
                        "%u\nMessage: %s\n", pkt->seqnum, pkt->msg);
                break;
            case(RETRANS):
                rec_retransmission(pkt, info);
                break;
            default:
                exit(EXIT_FAILURE);
                break;
        }
        memset(pkt, 0, PKTSZ);
    }
    return 0;
}
