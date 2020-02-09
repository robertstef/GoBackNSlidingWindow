/*
 * Robert Stefanyshin
 * ras867
 * 11137532
 * CMPT434
 * A2
 */

/*
 * This file defines the necessary structs, datatypes, macros,
 * and functions for sending, receiving, and packaging information
 * by both the sender and receiver
 */ 

#ifndef PACKET_H
#define PACKET_H

#include <time.h>

/* Macro definitions */
#define MAXBUF 100 // the max message size that can be sent by the
                   // sender and the receiver
#define ACKSZ (sizeof(uint)) // size of an ack
#define PKTSZ (sizeof(PKT)) // size of packet



/* Struct/type definitions */

/* Unsigned integer to be used as a sequence number */
typedef unsigned int uint;

/*
 * Defines a packet containing a user inputted
 * message to be sent from the sender to receiver.
 */
typedef struct packet
{
    uint seqnum;       // the sequence number of the packet
    char msg[MAXBUF];  // message to be sent to the receiver
    time_t sent;       // time message was sent
} PKT;



/* Function definitions */
/*
 * Creates a packet. Returns a pointer to the newly allocated
 * packet.
 *
 * Input:
 * uint seqnum: sequence number
 * char *msg: message
 *
 * Output:
 * PCK * on success, NULL on failure
 */
PKT *create_pkt(uint seqnum, char *msg);

/*
 * Logs the time a packet was sent over the network.
 *
 * Input:
 * PKT *: pointer to a packet
 *
 * Output:
 * 0 on success, -1 on failure
 */ 

#endif // PACKET_H
