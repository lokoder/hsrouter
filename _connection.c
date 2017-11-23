#include "_connection.h"
#include "_log.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
struct connection *new_connection(int sock, struct sockaddr_in *addr, node_type type) {

    struct connection *node = calloc(1, sizeof(struct connection));
    if (node == NULL) {
        err();
        d("calloc retornou NULL em _connection.h, new_connection()...\n");
        exit(0);
    }

    node->sock = sock;
    memcpy(&(node->addr), addr, sizeof(struct sockaddr));
    node->type = type;

    d("### lista: novo node criado\n");
    return node;
}
*/


/*
struct connection *add_connection(struct connection *list, struct connection *node) {

    d("### lista: conexao adicionada à lista\n");
    node->next = list;
    return node;
}
*/



/*
struct connection *remove_connection(struct connection *list, int sock) {
    
    struct connection *prev, *cur;
    prev = NULL;

    for (cur = list; cur != NULL; cur = cur->next) {

        if (cur->sock == sock) {
            
            if (prev == NULL) {
                
                d("### lista: conexao removida da lista\n");
                struct connection *tmp = cur->next;
                free(cur);
                return tmp;
                
            } else {
                
                d("### lista: conexao removida da lista\n");
                prev->next = cur->next;
                free(cur);
                return list;
            }
        }
        prev = cur;
    }

    d("### lista: nao encontrou o node para remover\n");
    return list;
}
*/




/*
struct connection *get_connection(struct connection *list, int sock) {

    struct connection *cur;

    for (cur = list; cur != NULL; cur = cur->next) {

        if (cur->sock == sock) {
            d("### lista: retornando connection procurada\n!");
            return cur;
        }
    }

    d("### lista: nao encontrou a connection procurada\n");
    return NULL;
}
*/



/*
struct connection *add_client(struct connection *list, int server_sock, struct connection *client) {

    struct connection *srv = get_connection(list, server_sock);
    if (srv == NULL) {
        err();
        return NULL;
    }        

    srv->client = add_connection(srv->client, client);
    return srv->client;
}
*/










/* refact */


struct server *server_new(int sock, struct sockaddr_in *addr) {
    
    struct server *node = calloc(1, sizeof(struct server));

    node->id = 0;
    node->sock = sock;
    node->clients = NULL;
    node->next = NULL; 
    memcpy(&(node->addr), addr, sizeof(struct sockaddr));
    
    return node;
}

struct server *server_add(struct server *list, struct server *node) {

    node->next = list;
    return node;
}

struct server *server_get(struct server *list, int sock) {

    struct server *cur;

    for (cur=list; cur!=NULL; cur=cur->next) {

        if (cur->sock == sock) {
            return cur;
        }
    }

    return NULL;
}


struct server *server_remove(struct server *list, int sock) {

    struct server *prev = NULL, *cur;

    for (cur = list; cur != NULL; cur = cur->next) {
        if (cur->sock == sock) {
            
            if (prev == NULL) {
                struct server *tmp = cur->next;
                free(cur);
                return tmp;

            } else {

                prev->next = cur->next;
                free(cur);
                return list;
            }
            prev = cur;
        }
    }
    return list;
}




struct client *client_new(int sock, struct sockaddr_in *addr) {

    struct client *node = calloc(1, sizeof(struct client));

    node->id = 0;
    node->sock = sock;
    node->server = NULL;
    memcpy(&(node->addr), addr, sizeof(struct sockaddr));

    return node;
}



struct server *client_add(struct server *server, struct client *new_client) {

    if (server->clients != NULL) {
        
        new_client->next = server->clients;
        server->clients = new_client;

    } else {
        
        server->clients = new_client;
    }

    return server;
}




struct client *client_remove(struct client *list, int sock) {

    struct client *prev = NULL, *cur;

    for (cur = list; cur != NULL; cur = cur->next) {
        if (cur->sock == sock) {
            
            if (prev == NULL) {
                struct client *tmp = cur->next;
                free(cur);
                return tmp;

            } else {

                prev->next = cur->next;
                free(cur);
                return list;
            }
            prev = cur;
        }
    }
    return list;
}
