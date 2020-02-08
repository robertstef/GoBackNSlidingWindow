/*
 * Robert Stefanyshin
 * ras867
 * 11137532
 * CMPT434
 * A2
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <string.h>
#include "queue.h"
#include "setup.h"
#include "packet.h"

#define MAX (base + wsize)

QUEUE *queue; // queue of packets waiting for acks and to be sent

uint base;  // lowest sequence number in sending window
uint wsize; // size of sending window
uint snum;  // current sequence number
uint max;   // maximum sequence number for the current window
uint ack;   // the next expected ack number

int sender_init(uint winsize)
{
    queue = queueCreate();
    if ( queue == NULL )
    {
        fprintf(stderr, "sender_init(): could not create queu\n");
        return -1;
    }

    base = 0;
    wsize = winsize;
    snum = 0;
    max = 0;
    ack = 0;

    return 0;
}

struct pollfd *setup_pfds(int sockfd)
{
    struct pollfd *pfds;

    if ( (pfds = malloc(sizeof(struct pollfd)*2)) == NULL )
    {
        fprintf(stderr, "setup_pfds(): malloc error\n");
        return NULL;
    }

    memset(pfds, 0, sizeof(*pfds));
    pfds[0].fd = STDIN_FILENO;
    pfds[0].events = POLLIN;

    pfds[1].fd = sockfd;
    pfds[1].events = POLLIN;

    return pfds;
}

int sender_input(PKT *pkt)
{
    int rv;

    // add packet to queue
    if ( (rv = enqueue(queue, pkt)) == -1 )
    {
        fprintf(stderr, "sender_input(): could not enqueue packet\n");
        return -1;
    }

    // if we can


    return 0;
}


