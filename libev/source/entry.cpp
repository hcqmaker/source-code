#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>
#include <cstdio>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "asyn_server.h"
#include "nb_net_tool.h"
#include "configuration.h"

int main()
{
    int server = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in   serv_addr;
    bzero(&serv_addr, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(10026);

    sigset_t     new_mask;
    sigset_t     old_mask;
    sigfillset(&new_mask);
    pthread_sigmask(SIG_BLOCK, &new_mask, &old_mask);
    printf("start binding\n");
    if (0 != bind(server, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))
    {
        printf("error happen when use bind\n");
    }

    printf("start listening\n");
    if (0 != listen(server, 10))
    {
        printf("error happen when use listen\n");
    }


    sigset_t    wait_mask;
    sigemptyset(&wait_mask);
    sigaddset(&wait_mask, SIGINT);
    sigaddset(&wait_mask, SIGQUIT);
    sigaddset(&wait_mask, SIGTERM);
    pthread_sigmask(SIG_BLOCK, &wait_mask, 0);
    int flag = 0;
    sigwait(&wait_mask, &flag);

    return 0;
}


