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
#include <time.h>
#include "queue.h"
#include "setup.h"
#include "packet.h"

#define MAX(a, b) (a + b)

QUEUE *queue; // queue of packets waiting for acks and to be sent

uint base;   // lowest sequence number in sending window
uint wsize;  // size of sending window
uint seqnum; // current sequence number
time_t timeout; // timeout time

int sender_init(uint winsize, int time)
{
    queue = queueCreate();
    if ( queue == NULL )
    {
        fprintf(stderr, "sender_init(): could not create queu\n");
        return -1;
    }

    base = 0;
    wsize = winsize;
    seqnum = 0;
    timeout = time;

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

int sender_input(char *input, SOCK_INFO *info)
{
    int rv;
    PKT *pkt;
    
    // create new packet
    if ( (pkt = create_pkt(seqnum, input)) == NULL )
        return -1;

    // sending window is not full
    if ( seqnum < MAX(base, wsize) )
    {
        // log send time
        pkt->sent = time(NULL);
        // send to receiver
        if ((rv = send_udp(pkt, PKTSZ, info)) == -1)
            return -1;
        // add to queue
        if ((rv = enqueue(queue, pkt)) == -1)
            return -1;
        seqnum++;
    }
    // sending window is full - add to queue
    else
    {
        if ((rv = enqueue(queue, pkt)) == -1)
            return-1;
        seqnum++;
    }

    return 0;
}

int sender_ack(uint ack, SOCK_INFO *info)
{
    PKT *pkt;
    int rv;
    NODE *n;

    // remove the message(s) being acked
    while( size(queue) > 0 )
    {
        // remove packet from queue
        pkt = peek(queue);
        if ( (rv = dequeue(queue)) == -1 )
            return -1;

        // if this is the ack we are looking for, stop
        if( pkt->seqnum == ack )
        {
            base = ack + 1;
            break;
        }
    }
    
    // if there are unsent messages in the queue, send as
    // many as we can
    if ( size(queue) > 0 )
    {
        // send as many unsent messages as we can
        n = queue->head;

        // locate first unsent message
        while( n != NULL )
        {
            pkt = n->item;
            if( pkt->sent == 0 )
                break;
            n = n->next;
        }

        // sent packts until we are at the end of the queue
        // or the sending window is full
        if ( n != NULL )
            pkt = n->item;
        else
            return 0;

        while( (n != NULL) && (pkt->seqnum < MAX(base, wsize)) )
        {
            pkt = n->item;
            // log send time
            pkt->sent = time(NULL);
            // send to receiver
            if ((rv = send_udp(pkt, PKTSZ, info)) == -1)
                return -1;

            n = n->next;
        }
    }

    return 0;
}

int sender_timeout(SOCK_INFO *info)
{
    int rv;
    PKT *pkt;
    NODE *n;

    if ( size(queue) > 0 )
    {
        n = queue->head;
        pkt = n->item;
        while( (n != NULL) && (pkt->seqnum < MAX(base, wsize)) )
        {
            pkt = n->item;

            // log send time
            pkt->sent = time(NULL);
            // send to receiver
            if ((rv = send_udp(pkt, PKTSZ, info)) == -1)
                return -1;

            n = n->next;
        }
    }
    return 0;
}

time_t calc_timeout(void)
{
    PKT *pkt;
    time_t last_unacked;
    time_t cur = time(NULL);
    time_t ans;
    
    if (size(queue) > 0 )
    {
        pkt = peek(queue);
        last_unacked = pkt->sent;
        ans = (timeout - (cur - last_unacked));
        if ( ans > 0 )
            return ans;
        else
            return timeout;
    }
    else
        return timeout;
}
