
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>

int main(int argc, char **argv){
int clientSocket;
char buffer[1024];
struct sockaddr_in serverAddr;
socklen_t addr_size;
unsigned short port;
struct hostent *hostnm;
uint32_t num, cnum;
char msg[30];
clientSocket = socket(PF_INET, SOCK_STREAM, 0);
serverAddr.sin_family = AF_INET;
port = (unsigned short)atoi(argv[2]);
serverAddr.sin_port = htons(port);
hostnm = gethostbyname(argv[1]);
serverAddr.sin_addr.s_addr = *((unsigned long *)hostnm->h_addr);
memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
addr_size = sizeof serverAddr;
connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);
// communication starts from here

    while(1){
        newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);
        /*---- Handle the incoming request in the newly created thread ----*/
      void handle_request(int newSocket){
    uint32_t network_choice;
    int choice;

    // Receive the choice from the client
    recv(newSocket, &network_choice, sizeof(network_choice), 0);
    choice = ntohl(network_choice);  // Convert from network byte order to host byte order

    switch (choice) {
        case 1:

            add(newSocket);
            break;
        case 2:

            display(newSocket);
            break;
        case 3:

            display_score(newSocket);
            break;
        case 4:
            display_all(newSocket);
            break;
        case 5:
            delete(newSocket);
            break;
        default:
            printf("Invalid choice\n");
            break;
    }

 
    close(newSocket);
}

    }
