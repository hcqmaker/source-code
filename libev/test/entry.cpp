#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>
#include <cstdio>

#include "asyn_server.h"
#include "nb_net_tool.h"

int main()
{
    int server = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in   serv_addr;
    bzero(&serv_addr, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(10026);

    if (0 != bind(server, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))
    {
        printf("error happen when use bind\n");
    }

    if (0 != listen(server, 10))
    {
        printf("error happen when use listen\n");
    }

    while (1)
    {
        printf("success\n");
    }
    return 0;
}
