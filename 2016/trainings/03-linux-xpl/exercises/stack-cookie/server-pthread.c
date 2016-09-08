#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

struct conn_data {
    int conn_fd;
    int listen_fd;
};


void *nossl_thread(void *arg)
{
    int fd, bread;
    char rbuff[64];
    struct conn_data *cdata;
    
    /* Retrieve arguments */
    cdata = (struct conn_data *)arg;
    fd = cdata->conn_fd;
    
    bzero(rbuff, sizeof(rbuff));
    

    while ((bread = read(fd, rbuff, 1024)) > 0) {
        write(fd, "Echo: ", 6);
        write(fd, rbuff, bread);
    }
    
    close(fd);
    free(cdata);
    return NULL;
}


int bind_port(int socket, uint16_t port, in_addr_t addr)
{
    struct sockaddr_in sin;
    
    bzero(&sin, sizeof(sin));
    sin.sin_addr.s_addr = addr;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    
    return bind(socket, (struct sockaddr *)&sin, sizeof(sin));
}

void daemon_mode()
{
    pthread_t thread;
    pthread_attr_t attr;
    
    int nconn, lconn;
    struct sockaddr client;
    struct conn_data *cdata;
    socklen_t addrlen = sizeof(client);
    
    lconn = socket(AF_INET, SOCK_STREAM, 0);
    if (bind_port(lconn, (uint16_t)414141, INADDR_ANY) ) {
        perror("bind(): ");
        return;
    }
    
    if (listen(lconn, 500)) {
        perror("listen(): ");
        return;
    }

    /* Set attributes for threads */
    if (pthread_attr_init(&attr))
        return;
    if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED))
        return;
    
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
        cdata = (struct conn_data *) malloc(sizeof(struct conn_data));
        cdata->conn_fd = nconn;
        cdata->listen_fd = lconn;

        if (pthread_create(&thread, &attr, nossl_thread, (void *)cdata)) {
            close(nconn);
            free(cdata);
        }
    }
}


int main(int argc, char const *argv[])
{
    daemon_mode();
    return 0;
}
