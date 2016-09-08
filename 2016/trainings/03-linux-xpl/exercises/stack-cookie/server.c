#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

struct conn_data {
    int conn_fd;
    int listen_fd;
};


int handle_connection(int fd)
{
    int bread;
    char rbuff[64];
    
    bzero(rbuff, sizeof(rbuff));

    if ((bread = read(fd, rbuff, 1024)) > 0) {

        if (write(fd, "Echo: ", 6) != 6)
            return -1;
        usleep(2000);
        if (write(fd, rbuff, bread) != bread)
            return -1;
        usleep(2000);
    }
    
    return bread;
}


int bind_port(int socket, uint16_t port, in_addr_t addr)
{
    struct sockaddr_in sin;
    int opt = 1;
    
    bzero(&sin, sizeof(sin));
    sin.sin_addr.s_addr = addr;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);

    setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    return bind(socket, (struct sockaddr *)&sin, sizeof(sin));
}

void daemon_mode()
{
    
    int nconn, lconn, opt = 1;
    struct sockaddr client;
    socklen_t addrlen = sizeof(client);
    
    lconn = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(lconn, SOL_TCP, TCP_NODELAY, &opt, sizeof(opt));
    if (bind_port(lconn, (uint16_t)414141, INADDR_ANY) ) {
        perror("bind(): ");
        return;
    }


    
    if (listen(lconn, 500)) {
        perror("listen(): ");
        return;
    }

    signal(SIGCHLD, SIG_IGN);
    
    for(;;) {
        
        fprintf(stderr, "Listening for new connection\n");
        bzero(&client, sizeof(client));
        nconn = accept(lconn, &client, &addrlen);
        if (nconn == -1) {
            perror("accept(): ");
            close(nconn);
            continue;
        }
        
        fprintf(stderr, "New incomming connection\n");
        setsockopt(nconn, SOL_TCP, TCP_NODELAY, &opt, sizeof(opt));

        if (fork() == 0) {
            close(lconn);
            while (handle_connection(nconn) > 0) {
                if (write(nconn, "Anything more?\n", 15) != 15)
                    break;
                usleep(2000);
            }
            close(nconn);
            exit(0);

        }

        close(nconn);
    }
}


int main(int argc, char const *argv[])
{
    register int ebp asm("ebp");

    printf("Cookie in ebp-0x0C: 0x%08x --> 0x%08x\n",
                ebp-0x0C, *(int *)(ebp-0x0C));

    daemon_mode();
    return 0;
}
