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
#include <fcntl.h>
#include <unistd.h>
#include "userinput.h"
#include "setup.h"
#include "packet.h"
#include "senderfcns.h"

#define FDCOUNT 2

int main(int argc, char *argv[])
{
    int rv, poll_rv;
    SOCK_INFO *info;
    struct addrinfo hints;
    char msg[MAXBUF];
    uint maxwindow, ack;
    int timeout;
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

    // get timeout and maxwindow from commandline
    maxwindow = (uint)atoi(argv[3]);
    timeout = atoi(argv[4]);
    
    // set up sender functions
    if ( (rv = sender_init(maxwindow, timeout)) == -1 )
        exit(EXIT_FAILURE);

    // set fds to non-blocking
    if ( (rv = fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK)) < 0 )
    {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
    if ( (rv = fcntl(info->sockfd, F_SETFL, O_NONBLOCK)) < 0 )
    {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }

    memset(msg, 0, MAXBUF);

    while(1)
    {
        poll_rv = poll(pfds, FDCOUNT, timeout * 1000);

        if ( poll_rv == -1 )
        {
            perror("poll");
            exit(EXIT_FAILURE);
        }

        if ( poll_rv == 0 )
        {
            printf("poll() timed out\n");
            rv = sender_timeout(info);
            if( rv == -1 )
                exit(EXIT_FAILURE);
            timeout = calc_timeout();
            printf("timeout: %d\n\n", timeout);
        }

        for(int i = 0; i < FDCOUNT; i++)
        {
            if ( pfds[i].revents & POLLIN )
            {
                // read to stdin
                if ( pfds[i].fd == STDIN_FILENO )
                {
                    // get input
                    read(STDIN_FILENO, msg, MAXBUF);

                    // send to receiver if possible, or place in queue
                    rv = sender_input(msg, info); 
                    if ( rv == -1 )
                        exit(EXIT_FAILURE);

                    // recalcuate timeout for poll 
                    timeout = calc_timeout();
                    printf("\n");
                    printf("timeout: %d\n\n", timeout);
                    memset(msg, 0, MAXBUF);
                }
                // we got an ack
                else if ( pfds[i].fd == info->sockfd )
                {
                    if ((rv = recv_udp(&ack, ACKSZ, info)) == -1)
                        exit(EXIT_FAILURE);

                    rv = sender_ack(ack);
                    if ( rv == -1 )
                        exit(EXIT_FAILURE);
                    printf("Got ack: %u\n\n", ack);
                }
            }
        }
    }

    return 0;
}
