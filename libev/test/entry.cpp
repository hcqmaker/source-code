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
#include <ev.h>

#include "asyn_server.h"
#include "nb_net_tool.h"

struct ev_loop*  loop = NULL;
int server = 0;

void accept_cb(EV_P_ ev_io* p, int enent)
{
    printf("accept  success\n");

    ev_io_stop(EV_A_ p);
    ev_break(EV_A_ EVBREAK_ONE);

    p = (ev_io*)malloc(sizeof(ev_io));
    ev_io_init(p, accept_cb, 0, EV_READ);
    ev_io_start(loop, p);
}


void cb(EV_P_ ev_io* p, int enent)
{
    printf("stdin read happen\n");
    //ev_io_stop(EV_A_ p);
    //ev_break(EV_A_ EVBREAK_ONE);
}

int main()
{
    // prepare io
    //struct ev_loop* loop  = EV_DEFAULT;
    loop  = EV_DEFAULT;
    ev_io   accept_id;
    ev_io   tmp;

    //int server = socket(AF_INET, SOCK_STREAM, 0);
    server = socket(AF_INET, SOCK_STREAM, 0);

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

    printf("ev io starting\n");
    ev_io_init(&accept_id, accept_cb, server, EV_READ);
    ev_io_start(loop, &accept_id);

    //ev_io_init(&tmp, cb, 0, EV_READ);
    //ev_io_start(loop, &tmp);

    ev_run(loop, 0);
    printf("\nev io end\n");

    sigset_t    wait_mask;
    sigemptyset(&wait_mask);
    sigaddset(&wait_mask, SIGINT);
    sigaddset(&wait_mask, SIGQUIT);
    sigaddset(&wait_mask, SIGTERM);
    pthread_sigmask(SIG_BLOCK, &wait_mask, 0);
    int flag = 0;
    sigwait(&wait_mask, &flag);

    printf("ev io stoping\n");
    ev_io_stop(EV_A_ &accept_id);
    ev_break(EV_A_ EVBREAK_ALL);
    printf("ev io has stoped\n");

    return 0;
}


