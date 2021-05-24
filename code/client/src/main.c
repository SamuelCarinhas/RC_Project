#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <pthread.h>

#define BUFLEN 1000

char endServer[100];
int fd, recv_len;
struct sockaddr_in addr;
struct hostent *hostPtr;
socklen_t slen = sizeof(addr);

void erro(char *msg);
void options();
void send_msg();
void send_2p2();
void multicast();
void leave();


int main(int argc, char *argv[]) {
    
    if(argc != 3){
        printf("Cliente <host> <port> \n");
        exit(-1);
    }

    strcpy(endServer, argv[1]);
    if((hostPtr = gethostbyname(endServer)) == 0){
        erro("Could not obtain host");
    }
    
    bzero((void *) &addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ((struct in_addr *)(hostPtr->h_addr))->s_addr;
    addr.sin_port = htons((short) atoi(argv[2]));

    //create UDP socket
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
        erro("Error opening socket");
    }

    //connects socket to receive data from sever
    if(connect(fd, (struct sockaddr*)&addr, slen) == -1){
        erro("Error conecting to server");
    }


    char username[16], password[16];

    while(1) {
        printf("Login menu:\n");
        printf("Username: ");
        scanf("%s", username);
        printf("Password: ");
        scanf("%s", password);

        char message[BUFLEN];
        snprintf(message, BUFLEN, "LOGIN %s %s", username, password);

        sendto(fd, message, BUFLEN, 0, (struct sockaddr *) &addr, (socklen_t) slen);

        int n = recvfrom(fd, message, BUFLEN, 0, (struct sockaddr *) &addr, &slen);
        if(n <= 0) {
            printf("Error contacting the server...\n");
            continue;
        }
        printf("%s", message);

        if(strcmp(message, "Login successfully\n") == 0) {
            break;
        }
    }

    //sendto(fd, argv[], BUFLEN, 0, (struct sockaddr *) &addr, (socklen_t) slen);

    options();

    close(fd);
    return 0;
}

void send_msg(){
    char username[16], message[100], msg[BUFLEN];
    printf("Send message to: ");
    scanf("%s", username);
    username[strcspn(username, "\n")] = '\0';
    
    while(fgetc(stdin) != '\n');

    printf("Message: ");
    scanf("%[^\n]s", message);
    snprintf(msg, BUFLEN, "MSG %s %s", username, message);

    sendto(fd, msg, BUFLEN, 0, (struct sockaddr *) &addr, slen);

    char response[BUFLEN];
    int n = recvfrom(fd, response, BUFLEN, 0, (struct sockaddr *) &addr, &slen);
    if(n <= 0) {
        printf("Error contacting the server...\n");
    }

    printf("%s", response);
}

void send_2p2(){
    char username[BUFLEN], message[100], msg[BUFLEN];
    printf("Send message to: ");
    scanf("%s", username);
    username[strcspn(username, "\n")] = '\0';

    printf("Message: ");
    scanf("%s", message);
    snprintf(msg, BUFLEN, "MSG %s %s", username, message);

    char response[BUFLEN];
    int n = recvfrom(fd, response, BUFLEN, 0, (struct sockaddr *) &addr, &slen);
    if(n <= 0) {
        printf("Error contacting the server...\n");
    }

    //struct sockaddr_in rcv;
    




    
}

void multicast(){

}

void options() {
    int option;

    printf("1 - Send message via Server\n");
    printf("2 - Send message via 2P2\n");
    printf("3 - Send group message\n");
    printf("4 - Exit\n");

    do{
        scanf("%d", &option);
    } while (1 > option || option > 4);
    
    switch(option){
        case 1:
            send_msg();
            break;
        case 2:
            send_2p2();
            break;
        case 3:
            multicast();
            break;
        case 4:
            printf("Leaving\n");
            leave();
            break;
    }
}




void leave(){
    //close socket
    close(fd);
    
    exit(0);
}

void erro(char *msg){
    printf("Erro: %s\n", msg);
    exit(-1);
}
