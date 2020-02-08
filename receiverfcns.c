/*
 * Robert Stefanyshin
 * ras867
 * 11137532
 * CMPT434
 * A2
 */

#include <stdio.h>
#include <string.h>
#include "userinput.h"
#include "receiverfcns.h"

int pkt_recvd(void)
{
    char msg[MAXIN];
    
    memset(msg, 0, MAXIN);

    // get response from user
    fprintf(stdout, "Receive the current packet (y/n)? ");
    fgets(msg, MAXIN, stdin);

    // parse response
    if ( (msg[0] == 'y') || (msg[0] == 'Y') )
        return 1;
    else if ( (msg[0] == 'n') || (msg[0] == 'N') )
        return 0;
    else
        return -1;
}

