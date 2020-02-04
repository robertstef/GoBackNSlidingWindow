/*
 * Robert Stefanyshin
 * ras867
 * 11137532
 * CMPT434
 * A2
 */

#include <stdio.h>
#include <stdlib.h>
#include "userinput.h"
#include "setup.h"

int main(int argc, char *argv[])
{
    int rv;

    if ( (rv = checkin_sender(argc, argv)) == -1 )
    {
        fprintf(stderr, "usage: sender ip port maxwindow timeout\n");
        exit(EXIT_FAILURE);
    }
    else
        fprintf(stderr, "input was valid\n");
    return 0;
}

