#include <packet.h>
#include <setup.h>

#define NEXT 0       // got next in order packet
#define OUTOFORDER 1 // got out of order packet
#define RETRANS 2    // got retransmission

/*
 * Initializes the receiver portion of the forwarding. Call 
 * before receiving any messages from sender.
 *
 * Input:
 * None
 *
 * Output:
 * None
 */
void fwd_rec_init(void);

/*
 * Checks the sequence number of the newly received packet
 * and indicates if the packet was next in order, out of
 * order, or a retransmission
 *
 * Input:
 * PKT *pkt: packet sent from sender
 *
 * Output:
 * NEXT, OUTOFORDER, RETRANS, -1 otherwise
 */ 
int fwd_parse_pkt(PKT *pkt);

/*
 * If the received packet was next in order, will print
 * it to stoud and prompt to user to keep or discard it.
 * If kept, will send an ack.
 * 
 * Input:
 * PKT *pkt: packet from sender
 * SOCK_INFO *info: senders socket information
 *
 * Output:
 * 0 on success, -1 on failure
 */ 
int fwd_rec_next(PKT *pkt);

/*
 * If the received packet was a retransmission of a
 * previously received packet will print the packet
 * to stdout indicating it was a retransmission and
 * update the last previously received packet. Will 
 * also send ack to sender.
 *
 * Input:
 * PKT *pkt: packet from sender
 * SOCK_INFO: sender socket information
 *
 * Output:
 * 0 on success, -1 on failure
 */
int fwd_rec_retransmission(PKT *pkt, SOCK_INFO *info);

/*
 * Prompts the user to indicate if the new packet will be
 * received or not. If the user enters a string starting
 * with Y or y, the packet will be received. If the user
 * enters a string starting with N or n, the packet will
 * be discarded.
 *
 * Input:
 * None
 *
 * Output:
 * 0 if no, 1 if yes, -1 otherwise
 */
int fwd_pkt_recvd(void);
