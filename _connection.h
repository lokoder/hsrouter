#ifndef CONNECTION_H_INCLUDED
#define CONNECTION_H_INCLUDED

#include <netinet/in.h>


/*
typedef enum {
    SERVER,
    CLIENT
} node_type;

struct connection {
    
    int sock;
    struct sockaddr_in addr;
    node_type type;
    int bytes_recv;
    int bytes_sent;
    struct connection *client;
    struct connection *next;
};


struct connection *new_connection(int sock, struct sockaddr_in *addr, node_type type);
struct connection *add_connection(struct connection *list, struct connection *node);
struct connection *remove_connection(struct connection *list, int sock);
struct connection *get_connection(struct connection *list, int sock);

*/




/* refact */

struct client {
    int id;
    int sock;
    struct sockaddr_in addr;
    struct server *server;
    struct client *next;
};


struct server {
    int id;
    int sock;
    struct sockaddr_in addr;
    struct client *clients;
    struct server *next;
};



struct server *server_new(int sock, struct sockaddr_in *addr);
struct server *server_add(struct server *list, struct server *node);
struct server *server_get(struct server *list, int sock);
struct server *server_remove(struct server *list, int sock);
struct client *client_new(int sock, struct sockaddr_in *addr);
struct server *client_add(struct server *server, struct client *new_client);
struct client *client_remove(struct client *list, int sock);


#endif





















