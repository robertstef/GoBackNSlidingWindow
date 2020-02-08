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
#include "receiverfcns.h"

#define PORT "30000"

int main()
{
    int rv;
    SOCK_INFO *info;
    struct addrinfo hints;
    char msg[MAXBUF];
    PKT *pkt;
    uint nxt_seq = 0; // seq num of next expected packet
    int retransmission;

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
    
    info->addr_len = sizeof(info->addr);

    // main receive loop
    while(1)
    {
        // get packet from sender
        rv = recv_udp(pkt, PKTSZ, info);
        if ( rv == -1 )
            exit(EXIT_FAILURE);
        
        // packet is next in-order message
        if ( pkt->seqnum == nxt_seq )
        {
            // packet is not retransmission
            if ( ! retransmission )
            {
                // print to screen
                fprintf(stdout, "Seq num: %u\nMessage: %s\n", 
                        pkt->seqnum, pkt->msg);

                // user decides if packet was received
                rv = pkt_recvd();
                
                // receive packet 
                if ( rv )
                {
                    fprintf(stdout, "receiving packet\n");
                    nxt_seq++;
                    retransmission = 0;
                    rv = send_udp(&pkt->seqnum, ACKSZ, info);
                    if ( rv == -1 )
                        exit(EXIT_FAILURE);
                }
                // discard packet
                else if ( !rv )
                {
                    fprintf(stdout, "discarding packet\n");
                    retransmission = 1; // we are expecting this to get
                                        // resent
                    // nxt_seq stays the same
                }
                else
                    fprintf(stdout, "error\n");
            }
            // packet is retransmission
            else if ( retransmission )
            {
                // print to screen indicating retrans
                fprintf(stdout, "RETRANSMISSION\nSeq num: %u\nMessage: %s\n",
                        pkt->seqnum, pkt->msg);

                retransmission = 0; // no longer waiting to have retransmitted
                nxt_seq++;
            }
            else
                fprintf(stderr, "Something is wrong\n");
        }
        // packet is out of order
        else
        {
            fprintf(stdout, "MESSAGE OUT OF ORDER\nSeq num: %u\nMessage: %s\n",
                    pkt->seqnum, pkt->msg);
        }

        memset(pkt, 0, PKTSZ);
    }
    return 0;
}
