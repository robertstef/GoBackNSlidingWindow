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
#include <arpa/inet.h>

int valid_IP(char *ip_addr)
{
    int rv;
    struct sockaddr_in ipv4;
    struct sockaddr_in ipv6;

    memset(&ipv4, 0, sizeof(ipv4));
    memset(&ipv6, 0, sizeof(ipv6));

    // IP is IPv4
    rv = inet_pton(AF_INET, ip_addr, &ipv4.sin_addr);
    if ( rv == 1 )
        return 0;

    // IP is IPv6
    rv = inet_pton(AF_INET6, ip_addr, &ipv6.sin_addr);
    if ( rv == 1 )
        return 0;
    
    // IP address was invalid
    return -1;
}

int valid_port(char *port)
{
    long int rv;
    char *ptr;

    if ( (rv = strtol(port, &ptr, 10)) == -1 )
        return -1;

    // port must be in range 30000-40000
    if ( rv < 30000 || rv > 40000 )
        return -1;

    return 0;
}

int valid_uint(char *uint)
{
    long int rv;
    char *ptr;

    if ( (rv = strtol(uint, &ptr, 0)) == -1 )
        return -1;

    if ( rv <= 0 )
        return -1;

    return 0;
}

int checkin_sender(int count, char *args[])
{
    int rv;
    
    // count must = 5
    if ( count != 5 )
        return -1;

    // IP
    if ( (rv = valid_IP(args[1])) == -1 )
    {
        fprintf(stderr, "ip must be a valid IPv4 or IPv6 address\n");
        return -1;
    }

    // PORT
    if ( (rv = valid_port(args[2])) == -1 )
    {
        fprintf(stderr, "port must be an integer in range 30000-40000\n");
        return -1;
    }

    // MAXWINDOW
    if ( (rv = valid_uint(args[3])) == -1 )
    {
        fprintf(stderr, "maxwindow must be an integer greater than 0\n");
        return -1;
    }

    // TIMEOUT
    if ( (rv = valid_uint(args[4])) == -1 )
    {
        fprintf(stderr, "timeout must be an integer greater than 0\n");
        return -1;
    }

    return 0;
}
    
