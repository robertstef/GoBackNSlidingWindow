/*
 * Robert Stefanyshin
 * ras867
 * 11137532
 * CMPT434
 * A2
 */

#define MAXIN 50 // max size of input when prompted by the receiver

/*
 * Checks if the input string is a valid IPv4
 * or IPv6 address.
 *
 * Input:
 * char *ip_addr: a string representing an IPv4 
 *                or IPv6 address
 * 
 * Output:
 * 0 on success, -1 on failure
 */ 
int valid_IP(char *ip_addr);

/*
 * Checks if the input string is a valid port
 * number in the range 30000 - 40000.
 *
 * Input:
 * char *port: a string representing a port number
 *
 * Output:
 * 0 on success, -1 on failure
 */ 
int valid_port(char *port);

/*
 * Checks if the input string is an
 * integer greater than 0.
 *
 * Input:
 * char *uint: a string representing an integer
 *           
 * Output:
 * 0 on success, -1 on failure
 */ 
int valid_uint(char *uint);

/*
 * For use by the sender application.
 * Checks if the command line arguments are
 * of the following form:
 *
 * sender ip port maxwindow timeout
 *
 * Where:
 * ip is an IPv4 or IPv6 address
 * port is an integer in range 30000 - 40000
 * maxwindow is an integer greater than 0
 * timeout is an integer greater than 0
 *
 * Input:
 * int count: argument count
 * char *args[]: argument vector
 *
 * Output:
 * 0 on success, -1 on failure
 */ 
int checkin_sender(int count, char *args[]);
