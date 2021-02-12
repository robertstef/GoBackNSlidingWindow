#ifndef SENDERFCNS_H
#define SENDERFCNS_H

#include <time.h>
#include "packet.h"
#include <setup.h>

/*
 * Initializes the sender application.
 *
 * Input:
 * uint wsize: size of sending window
 * int time: timeout time from user
 *
 * Output:
 * 0 on success, -1 on failure
 */
int sender_init(uint wsize, int time);

/*
 * Sets up the array of struct pollfd to use with
 * the poll function in the sender application.
 *
 * Input:
 * int sockfd: socket file descriptor
 *
 * Output:
 * Pointer to an array of struct pollfd, NULL on failure
 */
struct pollfd *setup_pfds(int sockfd);

/*
 * If the user inputted a message add a the message to the
 * queue of messages to be sent. If the max allowable size of
 * unacked messages has not been reached, the message will be
 * sent to the receiver. If the max allowable size of messages
 * has been reached, the message will wait in the queue until
 * it can be sent.
 *
 * Input:
 * char *input: user input
 * SOCK_INFO *int: socket information
 *
 * Output:
 * 0 on success, -1 on failure
 */
int sender_input(char *input, SOCK_INFO *info);

/*
 * If an ack was received from the receiver, will remove all
 * message from the queue up to and including the last message
 * we got an ack for.
 *
 * Input:
 * uint ack: ack sent from the receiver where the value is the
 *           seqence number of the message we are receiving the
 *           ack for
 *
 * Output:
 * 0 on success, -1 on failure
 */
int sender_ack(uint ack);

/*
 * If a timeout occurred, will resend all message in the
 * current sending window.
 *
 * Input:
 * SOCK_INFO *info:  socket infomation
 *
 * Output:
 * 0 on success, -1 on failure
 */ 
int sender_timeout(SOCK_INFO *info);

/*
 * Calculates the new timeout value for the next call to poll()
 * 
 * Input:
 * None
 *
 * Output:
 * new timeout value
 */
time_t calc_timeout(void);
#endif // SENDERFCNS_H
