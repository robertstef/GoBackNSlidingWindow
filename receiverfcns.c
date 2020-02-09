/*
 * Robert Stefanyshin
 * ras867
 * 11137532
 * CMPT434
 * A2
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "userinput.h"
#include "receiverfcns.h"
#include "packet.h"
#include "setup.h"
#include "queue.h"

uint nxt_sn;  // next expected sequence number
uint last_sn; // last received sequence number
int prob_ack; // probability of an ack being sent

void rec_init(int p_ack)
{
    last_sn = 0;
    nxt_sn = 0;
    prob_ack = p_ack;
}

int parse_pkt(PKT *pkt)
{
    // packet was next in order
    if ( pkt->seqnum == nxt_sn )
        return NEXT;
    else if ( pkt->seqnum == last_sn )
        return RETRANS;
    else
        return OUTOFORDER;
}

int rec_next(PKT *pkt, SOCK_INFO *info)
{
    int rv;

    // print to screen
    fprintf(stdout, "Seq num: %u\nMessage: %s\n", 
            pkt->seqnum, pkt->msg);

    // user decides if packet was received
    rv = pkt_recvd();

    // packet is to be received
    if (rv)
    {
        // decide of ack is sent or not
        rv = calc_prob();

        // yes - send the ack
        if (rv)
        {
            printf("Sending ack %u\n\n", pkt->seqnum);
            rv = send_udp(&pkt->seqnum, ACKSZ, info);
            if ( rv == -1 )
                return -1;
        }

        // increment next expect seq num
        nxt_sn++;

        // update last received sequence number
        last_sn = pkt->seqnum;
    }

    // packet is being discarded - do nothing

    return 0;
}

int rec_retransmission(PKT *pkt, SOCK_INFO *info)
{
    int rv;
    //PKT *last_rec;

    // update last received packet
    last_sn = pkt->seqnum;

    // print to screen
    fprintf(stdout, "RETRANSMISSION\nSeq num: %u\nMessage: %s\n",
            last_sn, pkt->msg);

    // calculate probability ack gets through
    rv = calc_prob();
    // if yes - send it
    if (rv)
    {
        rv = send_udp(&last_sn, ACKSZ, info);
        if ( rv == -1 )
            return -1;
    }
    // otherwise indicate ack wasn't sent
    else
        fprintf(stdout, "Ack for seqnum %u not sent\n\n", last_sn);

    return 0;
}

int pkt_recvd(void)
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

int calc_prob(void)
{
    int rv = rand() % 10; // get random number between 0 and 9

    if ( prob_ack < rv )
        return 0;
    // workout to test with acks always being sent
    else if ( prob_ack == 11 )
        return 1;
    else
        return 1;
}
