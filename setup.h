/*
 * Robert Stefanyshin
 * ras867
 * 11137532
 * CMPT434
 * A2
 */

#ifndef SETUP_H
#define SETUP_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/*
 * This struct is used when creating an active UDP socket
 * to store the infomation of the machine we are sending
 * to and the socket file descriptor we are sending over.
 */ 
typedef struct sock_info
{
    struct sockaddr addr; // address info
    socklen_t addr_len;    // address size
    int sockfd;            // socket file descriptor 
                           // we are sending over
} SOCK_INFO;

/*
 * This file contains function deifnitions that can be used
 * to set up both a basic TCP or UDP client and server
 */

/*
 * This function sets up a passive socket on the
 * local machine.
 *
 * Input:
 * struct addrinfo *: hints for getaddrinfo(), needs to have
 *                    all fields specified before passing into
 *                    this function
 * char *: the port number we will be connecting to
 *
 * Output:
 * The socket file descriptor on success, -1 otherwise.
 */
int set_passive_tcp(struct addrinfo *, char *);

/*
 * This function sets up an active TCP socket with the
 * specified machine.
 *
 * Input:
 * struct addrinfo *: hints for getaddrinfo(), needs to have all
 *                    fields specified before passing into
 *                    this function
 * char *: port number host is listening on
 * char *: name of machine we are connecting to
 *
 * Output:
 * The socket file descriptor on success, -1 otherwise
 */
int set_active_tcp(struct addrinfo *, char *, char *);

/*
 * This function sets up a passive UDP socket on the local
 * machine.
 *
 * Input:
 * struct addrinfo *: hints for getaddrinfo(), needs to have all fields
 *                    specified before passing into this function
 * char *: port number to bind to
 *
 * Output:
 * The socket file descriptor on success, -1 on failure
 */
int set_passive_udp(struct addrinfo *, char *);

/*
 * This function sets an active UDP socket to communicate
 * with a UDP socket on a different machine.
 *
 * Input:
 * struct addrinfo *: hints for getaddrinfo(), need to have all fields
 *                    specified before passing into this function
 * char *: port server is listening on
 * char *: server machine name
 *
 * Output:
 * Pointer to a SOCK_INFO struct on success, NULL on failure
 */
SOCK_INFO *set_active_udp(struct addrinfo *, char *, char *);

/*
 * This function sends a packet over a TCP connection.
 *
 * Input:
 * void *: packet to be sent
 * int: socket file descriptor
 * int: size of packet
 *
 * Output:
 * 0 on success, -1 on failure
 */
int send_tcp(void *, int, int);

/*
 * This function send a packet over a UDP connection.
 *
 * Input:
 * void *: packet to be sent
 * int: size of packet
 * SOCK_INFO *: pointer to SOCK_INFO struct
 *
 * Output:
 * 0 on success, -1 on failure
 */
int send_udp(void *, int, SOCK_INFO *);

/*
 * This function receives a packet over a TCP connection.
 *
 * Input:
 * void *: buffer to hold received packet
 * int: socket file descriptor
 * int: size of incoming packet
 *
 * Output:
 * 0 on success, -1 on failure
 */
int recv_tcp(void *, int, int);

/*
 * This function receives a packet over a UDP connection.
 *
 * Input:
 * void *: buffer to hold received packet
 * int: size of packed to be received
 * SOCK_INFO *: SOCK_INFO struct
 *
 * Output:
 * 0 on success, -1 on failure
 */
int recv_udp(void *, int, SOCK_INFO *);

#endif // SETUP_H
