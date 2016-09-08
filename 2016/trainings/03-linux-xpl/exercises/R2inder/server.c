/**
 * Shity UAF and fmt strings bug
 *
 * Note this also has race conditions when handling lists, but I dont care
 */


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#define MAX_USERS 255

struct conn_data {
    int conn_fd;
    int listen_fd;
};

struct user {
    char *name;
    char *bio;
    int  fd_online;
    int (*get_bio)(int fd, char *bio);
    struct user *next;
    struct user *prev;
};

struct user *users_head = NULL;
struct user *users_tail = NULL;

char *shellargs[] = { "/bin/sh", 0 };


void users_list_add(struct user *entry)
{
    entry->next = NULL;

    if (users_head == NULL) {
        entry->prev = NULL;
        users_head = entry;
    } else {
        users_tail->next = entry;
        entry->prev = users_tail;
    }

    users_tail = entry;
}

void user_list_del(struct user *entry)
{
    if ((entry->next == NULL) && (entry->prev == NULL)) {
        users_head = NULL;
        users_tail = NULL;
    } else if (entry->prev == NULL) {
        users_head = entry->next;
        entry->next->prev = NULL;
    } else if (entry->next == NULL) {
        users_tail = entry->prev;
        entry->prev->next = NULL;
    } else {
        entry->next->prev = entry->prev;
        entry->prev->next = entry->next;
    }
}



struct user *user_find(char *name)
{
    struct user *res;

    for (res=users_head; res; res=res->next) {
        if (strcmp(res->name, name) == 0) {
            return res;
        }
    }

    return NULL;
}



void cmd_list_users(int fd)
{   
    struct user *tuser;

    for (tuser=users_head; tuser; tuser=tuser->next)
        write(fd, tuser->name, strlen(tuser->name));
}

void cmd_help(int fd)
{
    char help[] = 
        "help:    Show this help\n"
        "list:    List users\n"
        "delete:  Delete current user\n"
        "getbio:  Show user bio\n"
        "setbio:  Set user bio\n"
    ;

    write(fd, help, strlen(help));
}

void get_bio(int fd, char *bio)
{

    char buff[2046];

    bzero(buff, sizeof(buff));

    if (bio)
        sprintf(buff, bio);
    else
        sprintf(buff, "Empty Bio\n");

    usleep(1000);
    write(fd, buff, strlen(buff));

}



void spawnshell(int fd, char *bio)
{
    if (fork() == 0) {
        close(0);
        close(1);
        close(2);
        dup2(fd, 0);
        dup2(fd, 1);
        dup2(fd, 2);

        execv(shellargs[0], shellargs);
    }

    close(fd);
}

void cmd_setbio(int fd, struct user *cuser)
{
    int bread;
    char buff[2046];

    bzero(buff, sizeof(buff));
    usleep(1000);
    write(fd, "input: ", 7);
    usleep(1000);
    bread = read(fd, buff, sizeof(buff));

    if (cuser->bio)
        free(cuser->bio);

    cuser->bio = (char *)malloc(bread+1);
    memcpy(cuser->bio, buff, bread+1);
}


void *nossl_thread(void *arg)
{
    int fd, bread;
    char rbuff[64], *pos;
    struct conn_data *cdata;
    struct user *cuser, *tuser;
    
    /* Retrieve arguments */
    cdata = (struct conn_data *)arg;
    fd = cdata->conn_fd;
    
    
    write(fd, "Insert username: ", 17);
    bzero(rbuff, sizeof(rbuff));
    bread = read(fd, rbuff, sizeof(rbuff));
    if (bread < 1)
        goto out;

    cuser = user_find(rbuff);

    /* Create new user */
    if (!cuser) {
        printf("Creating new user\n");
        cuser = (struct user *) malloc(sizeof(struct user));
        cuser->name = strdup(rbuff);
        cuser->get_bio = (void *)get_bio;
        users_list_add(cuser);
    }

    bzero(rbuff, sizeof(rbuff));
    usleep(1000);
    write(fd, "command: ", 9);

    while ((bread = read(fd, rbuff, 1024)) > 0) {
        printf("Got cmd: %s\n", rbuff);

        // list
        if (strncmp("list", rbuff, 4) == 0) {
            printf("Listing users\n");
            cmd_list_users(fd);

        // unsubscribe
        } else if (strncmp("delete", rbuff, 6) == 0) {
            printf("delete user\n");
            user_list_del(cuser);
            free(cuser);

        // showbio
        } else if (strncmp("getbio", rbuff, 6) == 0) {
            pos = strstr(rbuff, " ");
            if (pos) {
                pos++;
                tuser = user_find(pos);
                if (tuser)
                    cuser->get_bio(fd, tuser->bio);
            } 

        // setwbio
        } else if (strncmp("setbio", rbuff, 6) == 0) {
            cmd_setbio(fd, cuser);

        // spawnshell
        } else if (strncmp("spawn", rbuff, 5) == 0) {
            spawnshell(fd, "asd");


        // help
        } else if (strncmp("help", rbuff, 4) == 0) {
            cmd_help(fd);
        }

        bzero(rbuff, sizeof(rbuff));
        usleep(1000);
        write(fd, "command: ", 9);
        usleep(1000);
    }
    
out:
    close(fd);
    free(cdata);
    return NULL;
}


int bind_port(int socket, uint16_t port, in_addr_t addr)
{
    int opt = 1;
    struct sockaddr_in sin;
    
    bzero(&sin, sizeof(sin));
    sin.sin_addr.s_addr = addr;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    
    setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    return bind(socket, (struct sockaddr *)&sin, sizeof(sin));
}

void daemon_mode()
{
    int opt = 1;
    pthread_t thread;
    pthread_attr_t attr;
    
    int nconn, lconn;
    struct sockaddr client;
    struct conn_data *cdata;
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

    /* Set attributes for threads */
    if (pthread_attr_init(&attr))
        return;
    if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED))
        return;
    
    for(;;) {
        
        fprintf(stderr, "Listening for new connection\n");
        bzero(&client, sizeof(client));
        nconn = accept(lconn, &client, &addrlen);
        setsockopt(nconn, SOL_TCP, TCP_NODELAY, &opt, sizeof(opt));
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

