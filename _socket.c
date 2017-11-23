#include "_socket.h"


int bind_and_listen(int port) {

    int sock;
    struct sockaddr_in server={0};
   
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == -1) {
        err();
        return -1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

    int on=1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int)) < 0) {
        err();        
    }
    
    if (bind(sock, (struct sockaddr *)&server, sizeof(server)) == -1) {
        err();
        return -1;
    }

    if (listen(sock, 10) == -1) {
        err();
        return -1;
    }

    return sock;
}




int connect_to(char *ip) {

}

