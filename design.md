**<ins>Program design</ins>**

This implementation of a go-back-n sliding window protocol is divided between two separate applications, a sender and a receiver. The sender receives messages from the user, to simulate messages being sent from another process or machine, and sends the message to the receiver according to the rules of go-back-n. The receiver receives messages from the sender and sends acknowledgements (ack) according to the rules of go-back-n. The following two features have been implemented in the receiver to simulate corruption/loss of a packet and corruption/loss of an ack. First, when the next in order packet is received, the receiver will ask the user to keep or discard the packet to simulate message corruption/loss. Second, before the receiver sends an ack, the receiver will determine if the ack is sent based on the probability of corruption/loss inputted by the user. If the ack was not corrupted, it will be sent to the sender, otherwise the ack will not be sent.

**<ins>Description of source files</ins>**

1. `socketsetup.[ch]`
   
   Socketsetup.[ch] contain function calls for setting up active and passive UDP and TCP sockets. Additionally socketsetup.h defines a struct `SOCK_INFO` that is used to hold newly created socket file descriptors, and sender/receiver information when using UDP sockets.
   
2. `packet.[ch]`
   
   Packet.h defines a macro to for the maximum message size, `MAXBUF`, that can be entered by the user. When entering messages into the sender program, the maximum allowable message size is 100 characters. Additionally this file defines the macros `ACKSZ` and `PKTSZ`, that define the size of an ack and a packet respectively.
   
   Packet.h defines a struct `packet` that defines a packet. A packet contains
   3 fields, a sequence number, a message buffer, and a time stamp for when
   the message was sent to the receiver.

   Lastly, packet.[ch] defines a function that creates a new packet.
   
3. `userinput.[ch]`

   Userinput.[ch] defines a function `checkinput()` that is used to verify the following command line input when using the sender application:
   
  `sender IP port wsize timeout`
   
a) IP is a valid IPv4 or IPv6 address
   b) port is an integer in the range 30000-40000
   c) the window size is an integer greater than 0
   d) The timeout value is an integer greater than 0
   
4. `senderfcns.[ch]`

   These files contain the code that will carry out all necessary operations related to the go-back-n sliding window protocol for the sender application.

   Sender.c defines a queue of packets waiting for acks from the receiver or to be sent to the receiver. Additionally, sender.c defines the following variables: 

   > 1. `base`
   >
   >    Records the base sequence number of the current sending window
   >
   > 2. `wsize`
   >
   >    The size of the sending window, as defined by the user
   >
   > 3. `seqnum`
   >
   >    The sequence number of the next user inputted message
   >
   > 4. `timeout`
   >
   >    The timeout value for the sending window, as defined by the user
   >    This value is updated regularly to hold the timeout value for the
   >    oldest unacked message.	

   Sender.[ch] defines two setup functions. `Sender_init()` initializes the queue and variables described above. `Setup_pfds()` initializes an array of `struct pollfd` to be used by `poll()` in sender.c

   `Sender_input()` handles the case when a user enters a message. The message will be placed in a packet, along with a sequence number. If the sending window is not full, the message will be sent to the user and added to the queue of unacked/unsent messages. If the sending window is full, the message will be added to the queue and sent once its sequence number enters the sending window.

   `Sender_ack()` handles the case when the receiver sends an ack. The function will remove messages from the queue until the message with the sequence number corresponding to the ack is found. This allows for cumulative acks, as specified by the  go-back-n sliding window protocol. After the message(s) being acked have been removed from the queue, unsent message will be sent to the receiver either until there are no more unsent messages, or the sending window is full.

   `Sender_timeout()` handles the case when the sending window has timed out. This event occurs if the user has not entered a message and `poll()` has timed out, or the oldest unacked message has timed out. The function will iterate through the queue, starting with the oldest unacked message, and resend messages until the are no unsent messages or the sending window is full.

5. `receiverfcns.[ch]`

   Receiverfcns.[ch] contain the code that will carry out all necessary operations related to the go back-n sliding window protocol for the receiver application.
   
   Receiverfcns.c defines variables to track the following values: the next expected sequence number, the last received sequence number, and the probability an ack will be sent.

   `Rec_init()` initializes the above variables and is to be called in receiver.c before any messages are received or acks sent.
   
   `Parse_pkt()` takes a newly received packet and determines if it is the packet with the next expected sequence number, a retransmission of the last correctly received packet, or an out of order packet. The return value is then used by receiver.c to determine which operation to
execute.
   
   `Rec_next()` handles the case when the received packet contains the next in order sequence number. The sequence number and the message will be printed to stdout and the probability of the ack being sent is calculated. If yes, the ack is sent to the sender. If no, a message is printed to `stdout` indicating the ack for that sequence number was not sent.

   `Rec_retransmission()` handles the case where the sender sends a retransmission of the last correctly received packet. The packets message and sequence number, along with a message indicating it was a retransmission are printed to stdout. The probability of sending the ack is calculated. If yes, we sent the ack, otherwise a message is printed indicated the ack wasn't sent.
   
   `Pkt_recvd()` prompts the user in indicate if the packet is to be received. This function is used by `rec_next()` to determine whether to keep the current packet, or to discard it.
   
   `Calc_prob()` calculates the probability an ack will be sent to the sender.

6. `sender.c`

   This file contains the `main()` function to run the sender application. The first portion calls all setup functions. Then, `poll()` is called to monitor stdin and the newly created UDP socket. When an event occurs, the appropriate function from senderfcns.[ch] is called.
   
7. `receiver.c`

   This file contains the `main()` function to run the receiver application. The first portion calls all setup functions. The program then enters a main receive loop where packets are received from the sender. The new packet is then parsed by `parse_pkt()` and the correct operation
   is carried out by the functions defined in receiverfcns.c
   
8. `queue.[ch]`
  
   These files define a basic queue with the following operations: create, enqueue, dequeue, peek, size.