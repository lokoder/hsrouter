#include "_log.h"
#include "_socket.h"
#include "_connection.h"
#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

static const int BUFSIZE = 1024;


struct server *servers_list = NULL;



void *manage_client(void *arg) {
    
    struct client *me = arg;

    char buffer[BUFSIZE];
    int bytes;


    while (1) {
        bytes = recv(me->sock, buffer, BUFSIZE, 0);

        if (bytes < 0) { //erro... ver quais

            me->server->clients = client_remove(me->server->clients, me->sock);
            close(me->sock);
            d("bytes<0: erro de conexao...");
            pthread_exit(0);

        } else if (bytes == 0) { //o peer fechou a conexão! FIN
            
            me->server->clients = client_remove(me->server->clients, me->sock);
            close(me->sock);
            d("conexao encerrada pelo peer..\n");          
            pthread_exit(0); 
        }                

        printf("%ld - socket: %d - bytes: %d -  conteudo: %s\n", pthread_self(), me->sock, bytes, buffer);
        send(me->server->sock, buffer, bytes, 0);
        
        memset(buffer, 0, BUFSIZE);
    }
}




void *manage_server(void *arg) {
    
    struct server *me = arg;
    char buffer[BUFSIZE];
    int bytes;
    

    while (1) {

        bytes = recv(me->sock, buffer, BUFSIZE, 0);

        if (bytes < 0) { //erro... ver quais

            servers_list = server_remove(servers_list, me->sock);
            close(me->sock);
            d("bytes<0: erro de conexao...");
            pthread_exit(0);

        } else if (bytes == 0) { //o peer fechou a conexão! FIN
           
            servers_list = server_remove(servers_list, me->sock);
            close(me->sock); 
            d("conexao encerrada pelo peer..\n");
           pthread_exit(0);
        }

        struct client *c = NULL;
        for (c = me->clients; c != NULL; c = c->next) {
            send(c->sock, buffer, bytes, 0);
        }
        printf("%ld - socket: %d - bytes: %d -  conteudo: %s\n", pthread_self(), me->sock, bytes, buffer);
        memset(buffer, 0, BUFSIZE);
    }
}


void *process_request(void *arg) {

    char buf[BUFSIZE];
    int bytes=0;
    int sock = *((int *)arg);

    d("thread para gerenciar a nova conexao...");

    while (1) {

        memset(buf, 0, BUFSIZE);
        bytes = recv(sock, buf, BUFSIZE, 0);
        if (bytes < 0) {
            err();
            close(sock);
            pthread_exit(0);
        } else if (bytes == 0) {
            d("o peer encerrou a conexão!");
            close(sock);
            pthread_exit(0);
        }


        /* autenticação e binding!!! */
        if (!strncmp(buf, "server", 6)) {

            d("a nova conexao é de um SERVER, criando thread para gerenciar o server...");

            //realizar login ou cadastro

            struct sockaddr_in addr = {0};
            socklen_t len = sizeof(struct sockaddr);
            if (getpeername(sock, (struct sockaddr *)&addr, &len)) {
                err();
            }

            struct server *server = server_new(sock, &addr);
            servers_list = server_add(servers_list, server);

            pthread_t thread;
            pthread_create(&thread, NULL, manage_server, (void*)server);
            pthread_exit(0);

        } else if (!strncmp(buf, "client", 6)) {
            
            d("a conexao é de um CLIENT, criando thread para gerenciar o client...");

            struct sockaddr_in addr = {0};
            socklen_t len = sizeof(struct sockaddr);
            if (getpeername(sock, (struct sockaddr *)&addr, &len)) {
                err();
            }

            struct client *cli = client_new(sock, &addr);
            cli->server = servers_list;
            servers_list = client_add(servers_list, cli);
            

            //autenticar o cliente e associá-lo ao server
            pthread_t thread;
            pthread_create(&thread, NULL, manage_client, (void*)cli);
            pthread_exit(0);

        } else {
            
            d("o peer nao enviou um comando inicial valido, desconectando..");
            send(sock, "method not found", 16, 0);
            close(sock);
            pthread_exit(0);
        }
    }
}




int main() {

    struct sockaddr_in addr={0};
    unsigned int addrlen=sizeof(struct sockaddr_in);
    int sock = bind_and_listen(3999);

    while (1) {

        d("aceitando conexões na port 3999...");        
        int client = accept(sock, (struct sockaddr*)&addr, &addrlen);
        if (client == -1) {
            err();
            continue;
        }

        d("recebida nova conexao...");
        
        printf("socket n: %d ", client);
        printf("%s:%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

        d("criando nova thread para gerenciar a conexao...");
        pthread_t thread;
        if (pthread_create(&thread, NULL, process_request, (void*)&client)) {
            err();
            continue;
        }
        memset(&addr, 0, sizeof(addr));
    }
}

