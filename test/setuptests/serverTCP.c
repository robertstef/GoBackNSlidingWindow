#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setup.h>

#define PORT "30000"
#define BACKLOG 10
#define MAXBUF 100

int main()
{
    int rv, sockfd, newfd;
    struct addrinfo hints;
    struct sockaddr their_addr;
    socklen_t addr_len;
    char msg[MAXBUF];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ( (sockfd = set_passive_tcp(&hints, PORT)) == -1 )
        exit(EXIT_FAILURE);
    
    if (listen(sockfd, BACKLOG) == -1)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    addr_len = sizeof(their_addr);

    newfd = accept(sockfd, &their_addr, &addr_len);
    if ( newfd == -1 )
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        printf("server waiting for connections...\n");
        rv = recv_tcp(msg, newfd, MAXBUF);
        if ( rv == -1 )
            exit(EXIT_FAILURE);
        
        fprintf(stdout, "%s", msg);
    }
        
    return 0;
}
