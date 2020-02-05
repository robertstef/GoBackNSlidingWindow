/*
 * Robert Stefanyshin
 * ras867
 * 11137532
 * CMPT434
 * A2
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "setup.h"

#define BACKLOG 10 // number of pending connection queue can hold

int set_passive_tcp(struct addrinfo *hints, char *port)
{
    int rv;
    int sockfd;
    struct addrinfo *p, *servinfo;
    int yes = 1;

    if ((rv = getaddrinfo(NULL, port, hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return -1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                        p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                    sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)  {
		fprintf(stderr, "server: failed to bind\n");
        return -1;
	}

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
        return -1;
	}

    return sockfd;
}

int set_active_tcp(struct addrinfo *hints, char *port, char *name)
{
    int rv, sockfd;
    struct addrinfo *p, *servinfo;

    if ((rv = getaddrinfo(name, port, hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return -1;
	}

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			perror("client: connect");
			close(sockfd);
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return -1;
	}

    freeaddrinfo(servinfo);

    return sockfd;
}

int set_passive_udp(struct addrinfo *hints, char *port)
{
    int rv, sockfd;
    struct addrinfo *p, *servinfo;

    if ((rv = getaddrinfo(NULL, port, hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return -1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("listener: socket");
			continue;
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("listener: bind");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "listener: failed to bind socket\n");
		return -1;
	}

	freeaddrinfo(servinfo);

    return sockfd;
}

SOCK_INFO *set_active_udp(struct addrinfo *hints, char *port, char *name)
{
    int rv, sockfd;
    struct addrinfo *p, *servinfo;
    SOCK_INFO *result;

    if ( (result = malloc(sizeof(SOCK_INFO))) == NULL )
    {
        fprintf(stderr, "set_active_udp(): malloc error\n");
        return NULL;
    }

    if ((rv = getaddrinfo(name, port, hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return NULL;
    }

    // loop through all the results and make a socket
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                        p->ai_protocol)) == -1) {
            perror("talker: socket");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "talker: failed to create socket\n");
        return NULL;
    }

    // socket successfully created, package result and return
    result->addr = p->ai_addr;
    result->addr_len = p->ai_addrlen;
    result->sockfd = sockfd;

    return result;
}

int send_tcp(void *msg, int sockfd, int bufsize)
{
    int rv;

    while ( (rv = send(sockfd, msg, bufsize, 0) == -1) < bufsize )
    {
        if ( rv == -1 )
        {
            perror("send");
            return -1;
        }
    }
    return 0;
}

int send_udp(void *msg, int bufsize, SOCK_INFO *info)
{
   int rv;

   while ( (rv = sendto(info->sockfd, msg, bufsize, 0, info->addr, 
                   info->addr_len)) < bufsize )
   {
       if ( rv == -1 )
       {
           perror("sendto");
           return -1;
       }
   }
   return 0;
}

int recv_tcp(void *msg, int sockfd, int bufsize)
{
    int rv;

    while ( (rv = recv(sockfd, msg, bufsize, 0)) < bufsize )
    {
        if ( rv == -1 )
        {
            perror("recv");
            return -1;
        }
    }
    return 0;
}

int recv_udp(void *msg, int bufsize, SOCK_INFO *info)
{
    int rv;

    while ( (rv = recvfrom(info->sockfd, msg, bufsize, 0,
                    info->addr, &info->addr_len)) < bufsize )
    {
        if ( rv == -1 )
        {
            perror("recvfrom");
            return -1;
        }
    }
    return 0;
}

