#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fwdrec.h"
#include "packet.h"
#include <setup.h>
#include <userinput.h>

uint nxt_sn;  // next expected sequence number
uint last_sn; // last received sequence number

void fwd_rec_init(void)
{
    last_sn = 0;
    nxt_sn = 0;
}

int fwd_parse_pkt(PKT *pkt)
{
    // packet was next in order
    if ( pkt->seqnum == nxt_sn )
        return NEXT;
    else if ( pkt->seqnum == last_sn )
        return RETRANS;
    else
        return OUTOFORDER;
}

int fwd_rec_next(PKT *pkt)
{
    int rv;

    // print to screen
    fprintf(stdout, "Seq num: %u\nMessage: %s\n", 
            pkt->seqnum, pkt->msg);

    // user decides if packet was received
    rv = fwd_pkt_recvd();

    // packet is to be received - pass along to sender
    if (rv)
    {
        // increment next expect seq num
        nxt_sn++;

        // update last received sequence number
        last_sn = pkt->seqnum;

        return 1; // indicates we need to forward packet
    }

    // packet is being discarded - indicate with return 0
    return 0;
}

int fwd_rec_retransmission(PKT *pkt, SOCK_INFO *info)
{
    int rv;

    // update last received packet
    last_sn = pkt->seqnum;

    // print to screen
    fprintf(stdout, "RETRANSMISSION\nSeq num: %u\nMessage: %s\n",
            last_sn, pkt->msg);

    // pass along to receiver
    rv = send_udp(pkt, PKTSZ, info);
    if ( rv == -1 )
        return -1;
    return 0;
}

int fwd_pkt_recvd(void)
{
    char msg[MAXIN];
    
    memset(msg, 0, MAXIN);

    // get response from user
    fprintf(stdout, "Receive the current packet (y/n)? ");
    fgets(msg, MAXIN, stdin);
    printf("\n");

    // parse response
    if ( (msg[0] == 'y') || (msg[0] == 'Y') )
        return 1;
    else if ( (msg[0] == 'n') || (msg[0] == 'N') )
        return 0;
    else
        return -1;
}
