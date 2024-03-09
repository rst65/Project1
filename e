#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define MAX_STUDENTS 100

struct Student {
    int ID;
    char Fname[10];
    char Lname[10];
    float score; };

struct Student students[MAX_STUDENTS];
int numStudents = 0;

void add(int newSocket) {
    char buffer[1024];

    if (numStudents >= MAX_STUDENTS) {
        char *message = "Database is full.\n";
        send(newSocket, message, strlen(message), 0);
        return; }

    struct Student newStudent;

    char *prompt = "Enter student ID: ";
    send(newSocket, prompt, strlen(prompt), 0);
    recv(newSocket, buffer, 1024, 0);
    sscanf(buffer, "%d", &newStudent.ID);

    prompt = "Enter first name: ";
    send(newSocket, prompt, strlen(prompt), 0);
    recv(newSocket, buffer, 1024, 0);
    sscanf(buffer, "%s", newStudent.Fname);

    prompt = "Enter last name: ";
    send(newSocket, prompt, strlen(prompt), 0);
    recv(newSocket, buffer, 1024, 0);
    sscanf(buffer, "%s", newStudent.Lname);

    prompt = "Enter score: ";
    send(newSocket, prompt, strlen(prompt), 0);
    recv(newSocket, buffer, 1024, 0);
    sscanf(buffer, "%f", &newStudent.score);

    students[numStudents] = newStudent;
    numStudents++;

    FILE *file = fopen("students.txt", "a");
    if (file != NULL) {
        fprintf(file, "ID: %d, Name: %s %s, Score: %.2f\n", newStudent.ID, newStudent.Fname, newStudent.Lname, newStudent.score);
        fclose(file); }

    char *successMessage = "Student added successfully!\n";
    send(newSocket, successMessage, strlen(successMessage), 0);}


int main(int argc, char **argv){
int welcomeSocket, newSocket;
char buffer[1024];
/* sockaddr_in is an IPv4-specific address structure used for storing internet
addresses. */
struct sockaddr_in serverAddr;
/* sockaddr_storage is a generic address structure used for storing addresses of
various types, such as IPv4 and IPv6. */
struct sockaddr_storage serverStorage;
socklen_t addr_size;
unsigned short port; /* port server binds to */
uint32_t num;
char msg[30];
/*---- Create the socket. The three arguments are: ----*/
/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
/*---- Configure settings of the server address struct ----*/
/* Address family = Internet */
serverAddr.sin_family = AF_INET;
/* Set port number, using htons function to use proper byte order */
port = (unsigned short) atoi(argv[1]);
serverAddr.sin_port = htons(port);
/* Set IP address to localhost */
serverAddr.sin_addr.s_addr = INADDR_ANY;
/* Set all bits of the padding field to 0 */
memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
/*---- Bind the address struct to the socket ----*/
bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
/*---- Listen on the socket, with 5 max connection requests queued ----*/
if(listen(welcomeSocket,5)==0)
printf("Listening\n");
else
printf("Error\n");
/*---- Accept call creates a new socket for the incoming connection ----*/
addr_size = sizeof serverStorage;
newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage,
&addr_size);
// communication starts from here
// receive an integer from the client
recv(newSocket, &num, sizeof(num), 0);
printf("Integer received: %d\n",ntohl(num));
// send a reply message to the client
strcpy(msg, "Integer received");
send(newSocket, msg, sizeof(msg), 0);
close(newSocket);
close(welcomeSocket);
return 0;
}
