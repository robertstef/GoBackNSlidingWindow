/*
 * Robert Stefanyshin
 * ras867
 * 11137532
 * CMPT434
 * A2
 */

#ifndef FWDSEND_H
#define FWDSEND_H

#include <poll.h>
#include <time.h>
#include "packet.h"
#include "setup.h"

/*
 * Initializes sender portion of the forwarder.
 *
 * Input:
 * uint wsize: size of sending window
 * int time: timeout time from user
 *
 * Output:
 * 0 on success, -1 on failure
 */
int fwd_send_init(uint wsize, int time);

/*
 * Sets up the array of struct pollfd to use with
 * the poll function in the forwarder application.
 *
 * Input:
 * int rec_fd: socket fd for listening
 * int send_fd: socket fd for receiving
 *
 * Output:
 * Pointer to an array of struct pollfd, NULL on failure
 */
struct pollfd *fwd_setup_pfds(int rec_fd, int send_fd);

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
int fwd_input(PKT *input, SOCK_INFO *info);

/*
 * If an ack was received from the receiver, will remove all
 * message from the queue up to and including the last message
 * we got an ack for.
 *
 * Input:
 * SOCK_INFO *sndinfo: senders socket info
 * SOCK_INFO *recinfo: receivers socket info
 * uint ack: ack sent from the receiver where the value is the
 *           seqence number of the message we are receiving the
 *           ack for
 *
 * Output:
 * 0 on success, -1 on failure
 */
int fwd_ack(uint ack, SOCK_INFO *sndinfo, SOCK_INFO *recinfo);

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
int fwd_timeout(SOCK_INFO *info);

/*
 * Calculates the new timeout value for the next call to poll()
 * 
 * Input:
 * None
 *
 * Output:
 * new timeout value
 */
time_t fwd_calc_timeout(void);

#endif // FWDSEND_H
