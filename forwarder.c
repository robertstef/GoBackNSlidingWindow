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

#define PORT "35000" // port we are listening on

int main(int argc, char *argv[])
{
    int rv;
    SOCK_INFO *rcvr_info;   // info for the receiver application
    SOCK_INFO *sender_info; // info for the sender application
    struct addrinfo hints;
    PKT *pkt;
    uint ack;

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
    if ( (sender_info = malloc(sizeof(SOCK_INFO))) == NULL )
    {
        printf("forwarder: malloc\n");
        exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    if( (sender_info->sockfd = set_passive_udp(&hints, PORT)) == -1 )
        exit(EXIT_FAILURE);
    
    sender_info->addr_len = sizeof(sender_info->addr);



    // setup active socket to communicate with receiver app
    if ( (rcvr_info = malloc(sizeof(SOCK_INFO))) == NULL )
    {
        printf("forwarder: malloc\n");
        exit(EXIT_FAILURE);
    }
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ( (rcvr_info = set_active_udp(&hints, argv[2], argv[1])) == NULL )
        exit(EXIT_FAILURE);

    rcvr_info->addr_len = sizeof(rcvr_info->addr);

    while(1)
    {
        // receive packet from sender
        rv = recv_udp(pkt, PKTSZ, sender_info);
        if ( rv == -1 )
            exit(EXIT_FAILURE);

        // send packet to receiver
        rv = send_udp(pkt, PKTSZ, rcvr_info);
        if ( rv == -1 )
            exit(EXIT_FAILURE);

        // get ack from receiver
        rv = recv_udp(&ack, ACKSZ, rcvr_info);
        if ( rv == -1 )
            exit(EXIT_FAILURE);

        // send ack to sender
        rv = send_udp(&ack, ACKSZ, sender_info);
        memset(pkt, 0, PKTSZ);
    }

    return 0;
}
