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
#include <string.h>
#include "fwdsend.h"
#include "queue.h"
#include "setup.h"
#include "packet.h"

#define MAX(a, b) (a + b)

QUEUE *queue;

uint base;     // bottom of sending window
uint wsize;    // size of sending window
uint seqnum;   // current sequence number
time_t timeout;// timeout time

int fwd_send_init(uint winsize, int time)
{
    queue = queueCreate();
    if ( queue == NULL )
    {
        fprintf(stderr, "fwd_send_init(): could not create queu\n");
        return -1;
    }

    base = 0;
    wsize = winsize;
    seqnum = 0;
    timeout = time;

    return 0;
}

struct pollfd *fwd_setup_pfds(int rec_fd, int send_fd)
{
    struct pollfd *pfds;

    if ( (pfds = malloc(sizeof(struct pollfd)*2)) == NULL )
    {
        fprintf(stderr, "setup_pfds(): malloc error\n");
        return NULL;
    }

    memset(pfds, 0, sizeof(*pfds));
    pfds[0].fd = rec_fd;
    pfds[0].events = POLLIN;

    pfds[1].fd = send_fd;
    pfds[1].events = POLLIN;

    return pfds;
}

int fwd_input(PKT *input, SOCK_INFO *info)
{
    int rv;
    PKT *pkt;
    
    // create new packet
    if ( (pkt = create_pkt(seqnum, input->msg)) == NULL )
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

int fwd_ack(uint ack, SOCK_INFO *sndinfo, SOCK_INFO *recinfo)
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
            if ((rv = send_udp(pkt, PKTSZ, recinfo)) == -1)
                return -1;

            n = n->next;
        }
    }

    // forward ack to sender
    if ( (rv = send_udp(&ack, ACKSZ, sndinfo)) == -1 )
        return -1;

    return 0;
}

int fwd_timeout(SOCK_INFO *info)
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

time_t fwd_calc_timeout(void)
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
