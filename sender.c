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
#include <poll.h>
#include "setup.h"
#include "packet.h"
#include "senderfcns.h"

int main(int argc, char *argv[])
{
    int rv;
    SOCK_INFO *info;
    struct addrinfo hints;
    char msg[MAXBUF];
    PKT *pkt;
    uint seqnum = 0;
    uint timeout;
    struct pollfd *pfds;

    // check command line input
    if ( (rv = checkin_sender(argc, argv)) == -1 )
    {
        fprintf(stderr, "usage: sender IP port maxwindow timeout\n");
        exit(EXIT_FAILURE);
    }

    // setup hints
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    // create socket
    if ( (info = set_active_udp(&hints, argv[2], argv[1])) == NULL )
        exit(EXIT_FAILURE);

    // setup array of pollfds
    if ( (pfds = setup_pfds(info->sockfd)) == NULL )
        exit(EXIT_FAILURE);

    memset(msg, 0, MAXBUF);
    while(1)
    {
        printf(">> ");
        fgets(msg, MAXBUF, stdin);
        
        pkt = create_pkt(seqnum, msg);

        rv = send_udp(pkt, PKTSZ, info);
        if ( rv == -1 )
            exit(EXIT_FAILURE);
        seqnum++;
    }

    return 0;
}
