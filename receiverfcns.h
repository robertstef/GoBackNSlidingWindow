#include <packet.h>
#include <setup.h>

/* Define the three possible situations that can occur
 * when receiving a new packet from the sender.
 */ 
#define NEXT 0        // next in order packet
#define OUTOFORDER 1 // out of order packet
#define RETRANS 2     // retransmission of last correctly
                      // received in order message


/*
 * Initializes the receiver. Call before receiving any
 * messages from sender.
 *
 * Input:
 * int p_ack: probability ack will be corrupted
 *
 * Output:
 * None
 */
void rec_init(int p_ack);

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
int parse_pkt(PKT *pkt);

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
int rec_next(PKT *pkt, SOCK_INFO *info);

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
int rec_retransmission(PKT *pkt, SOCK_INFO *info);

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
int pkt_recvd(void);

/*
 * Indicates if the ack will be sent to the sender
 * or not. Is to be used only inside receiverfcns.c.
 *
 * Output:
 * 0 if no, 1 if yes
 */
int calc_prob(void);
