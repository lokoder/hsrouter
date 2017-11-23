#include "_connection.h"
#include "_log.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



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


