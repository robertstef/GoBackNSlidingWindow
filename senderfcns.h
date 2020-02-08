/*
 * Robert Stefanyshin
 * ras867
 * 11137532
 * CMPT434
 * A2
 */

#ifndef SENDERFCNS_H
#define SENDERFCNS_H

#include "packet.h"

/*
 * Initializes the sender application.
 *
 * Input:
 * uint wsize: size of sending window
 *
 * Output:
 * 0 on success, -1 on failure
 */
int sender_init(uint wsize);

/*
 * Sets up the array of struct pollfd to use with
 * the poll function/
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
 * PKT *pkt: packet to be enqueued
 * uint base: first sequence number in the sending window
 * uint winsize: size of the sending window
 *
 * Output:
 * 0 on success, -1 on failure
 */
int sender_input(PKT *pkt);

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
 * uint base: first sequence number in the sending window
 * uint winsize: size of sending window
 *
 * Output:
 * 0 on success, -1 on failure
 */ 
int sender_timeout(uint base, uint winsize);
#endif // SENDERFCNS_H
