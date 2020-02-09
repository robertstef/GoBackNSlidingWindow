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
#include "packet.h"

PKT *create_pkt(unsigned int seqnum, char *msg)
{
    PKT *pkt;
    char *retstr;

    // allocate space for packet
    if ( (pkt = malloc(sizeof(PKT))) == NULL )
    {
        fprintf(stderr, "create_pkt(): malloc error\n");
        return NULL;
    }

    // record sequence number
    pkt->seqnum = seqnum;

    // copy message into packet
    retstr = strcpy(pkt->msg, msg);
    if ( retstr == NULL )
    {
        fprintf(stderr, "create_pkt(): strcpy error\n");
        return NULL;
    }

    // set sent time to 0
    pkt->sent = 0;

    return pkt;
}

