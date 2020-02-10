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
#include <poll.h>
#include <fcntl.h>
#include "setup.h"
#include "packet.h"
#include "userinput.h"
#include "fwdsend.h"
#include "fwdrec.h"

#define PORT "35000" // port we are listening on
#define FDCOUNT 2

int main(int argc, char *argv[])
{
    int rv, timeout, pollrv;
    SOCK_INFO *rcvr_info;   // info for the receiver application
    SOCK_INFO *sender_info; // info for the sender application
    struct addrinfo hints;
    PKT *pkt;
    uint ack, maxwindow;
    char msg[MAXIN];
    struct pollfd *pfds;

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


    // get timeout and maxwindow from commandline
    maxwindow = (uint)atoi(argv[3]);
    timeout = atoi(argv[4]);
    
    // setup sender and receiver portions of forwarder
    fwd_send_init(maxwindow, timeout);
    fwd_rec_init();

    // set fds to non-blocking
    if ( (rv = fcntl(sender_info->sockfd, F_SETFL, O_NONBLOCK)) < 0 )
    {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
    if ( (rv = fcntl(rcvr_info->sockfd, F_SETFL, O_NONBLOCK)) < 0 )
    {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }

    // setup array of pollfds
    if ( (pfds = fwd_setup_pfds(sender_info->sockfd, 
                    rcvr_info->sockfd)) == NULL )
        exit(EXIT_FAILURE);
    
    memset(msg, 0, MAXIN);

    while(1)
    {
        pollrv = poll(pfds, FDCOUNT, timeout * 1000);

        if ( pollrv == -1 )
        {
            perror("poll");
            exit(EXIT_FAILURE);
        }

        if ( pollrv == 0 )
        {
            printf("\nPoll() timed out\n");
            timeout = fwd_calc_timeout();
        }

        for(int i = 0; i < FDCOUNT; i++)
        {
            if ( pfds[i].revents & POLLIN )
            {
                // got message from sender
                if ( pfds[i].fd == sender_info->sockfd )
                {
                    // receive message
                    if ((rv = recv_udp(pkt, PKTSZ, sender_info)) == -1)
                        exit(EXIT_FAILURE);

                    // parse the packet
                    rv = fwd_parse_pkt(pkt);

                    switch(rv)
                    {
                        // received the next in order message
                        case(NEXT):
                            // get receiver to process packet
                            if ( (rv = fwd_rec_next(pkt)) == -1 )
                                exit(EXIT_FAILURE);

                            // pass packet to sender
                            rv = fwd_input(pkt, rcvr_info);
                            if ( rv == -1 )
                                exit(EXIT_FAILURE);

                            // relcaculate timeout
                            timeout = fwd_calc_timeout();
                            memset(pkt, 0, PKTSZ);
                            break;

                        // message was out of order
                        case(OUTOFORDER):
                            fprintf(stdout, "MESSAGE OUT OF ORDER\nSeq num:" 
                                    "%u\nMessage: %s\n", pkt->seqnum, pkt->msg);
                            memset(pkt, 0, PKTSZ);
                            break;

                        // message was a retransmission
                        case(RETRANS):
                            fwd_rec_retransmission(pkt, rcvr_info);
                            // print to stoud
                            // forward packet to receiver
                            break;
                        default:
                            exit(EXIT_FAILURE);
                            break;
                    }

                }
                // got something from receiver
                else if ( pfds[i].fd == rcvr_info->sockfd )
                {
                    if ((rv = recv_udp(&ack, ACKSZ, rcvr_info)) == -1)
                        exit(EXIT_FAILURE);

                    //rv = sender_ack(ack);
                    if ( rv == -1 )
                        exit(EXIT_FAILURE);
                }
            }
        }
        /*
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
        */
        memset(pkt, 0, PKTSZ);
    }

    return 0;
}
