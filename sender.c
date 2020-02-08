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

int main(int argc, char *argv[])
{
    int rv;
    SOCK_INFO *info;
    struct addrinfo hints;
    char msg[MAXBUF];
    PKT *pkt;
    uint seqnum = 0;

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
        
        pkt = create_pkt(seqnum, msg);

        rv = send_udp(pkt, PKTSZ, info);
        if ( rv == -1 )
            exit(EXIT_FAILURE);
        seqnum++;
    }

    return 0;
}
