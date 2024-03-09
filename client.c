#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#define MAX_STUDENTS 100

struct Student {
    int ID;
    char Fname[10];
    char Lname[10];
    float score;};

struct Student students[MAX_STUDENTS];
int numStudents = 0;

void add(int clientSocket){
 char buffer[1024];
 struct Student newStudent;
    recv(clientSocket, buffer, sizeof(buffer)-1, 0);  
    printf("%s", buffer);
    scanf("%d", &newStudent.ID); 
    sprintf(buffer, "%d", newStudent.ID);  
    send(clientSocket, buffer, strlen(buffer) + 1, 0);  
    recv(clientSocket, buffer, sizeof(buffer)-1, 0);  
    printf("%s", buffer);
    scanf("%s", newStudent.Fname); 
    send(clientSocket, newStudent.Fname, strlen(newStudent.Fname)+1, 0);  
    recv(clientSocket, buffer, sizeof(buffer)-1, 0);  
    printf("%s", buffer);
    scanf("%s", newStudent.Lname); 
    send(clientSocket, newStudent.Lname, strlen(newStudent.Lname)+1, 0); 
    recv(clientSocket, buffer, sizeof(buffer)-1, 0);  
    printf("%s", buffer);
    scanf("%f", &newStudent.score);  
    sprintf(buffer, "%f", newStudent.score); 
    send(clientSocket, buffer, strlen(buffer) + 1, 0);  
    recv(clientSocket, buffer, sizeof(buffer)-1, 0);  
    printf("%s", buffer);}

void displayByID(int clientSocket) {
    char buffer[1024];
    int ID;
    printf("Enter student ID: ");
    scanf("%d", &ID);

    sprintf(buffer, "%d", ID);
    send(clientSocket, buffer, strlen(buffer) + 1, 0);
    recv(clientSocket, buffer, sizeof(buffer)-1, 0);

    if (strcmp(buffer, "No student was found.\n") == 0) {
        printf("%s", buffer);} 
    else {
        int ID;
        char Fname[10];
        char Lname[10];
        float score;
        sscanf(buffer, "ID: %d, Name: %s %s, Score: %.2f\n", &ID, Fname, Lname, &score);
        printf("ID: %d, Name: %s %s, Score: %.2f\n", ID, Fname, Lname, score);}}

void displayByScore(int clientSocket) {
    char buffer[1024];
    float scoreThreshold;

    printf("Enter score: ");
    scanf("%f", &scoreThreshold);

    sprintf(buffer, "%f", scoreThreshold);
    send(clientSocket, buffer, strlen(buffer) + 1, 0);

    while (recv(clientSocket, buffer, sizeof(buffer)-1, 0) > 0) {
        printf("%s", buffer); }}

void displayAll(int clientSocket) {
    char buffer[1024];

    char *request = "Display All";
    send(clientSocket, request, strlen(request) + 1, 0);
    while (recv(clientSocket, buffer, sizeof(buffer)-1, 0) > 0) {
        printf("%s", buffer);}}

void deleteByID(int clientSocket) {
    char buffer[1024];
    int ID;
    printf("Enter student ID to delete: ");
    scanf("%d", &ID);

    sprintf(buffer, "%d", ID);
    send(clientSocket, buffer, strlen(buffer) + 1, 0);

    recv(clientSocket, buffer, sizeof(buffer)-1, 0);
    printf("%s", buffer);}


int main(int argc, char **argv){
    int clientSocket;
    char buffer[1024];
    struct sockaddr_in serverAddr;
    socklen_t addr_size;
    unsigned short port;
    struct hostent *hostnm;
    uint32_t cnum;
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
    // send an integer to the server

    int choice;
  do{  printf("1. Add student\n2. Display student by ID\n3. Display students by score\n4. Display all students\n5. Delete student by ID\n6. Exit\nEnter Choice:");
    scanf("%d", &choice);
    uint32_t cchoice = htonl(choice);
    send(clientSocket, &cchoice, sizeof(cchoice), 0);

  switch (choice) {
    case 1: //Adds student to data and txt file
         add(clientSocket);
         break;
    case 2: //Display students by ID
         displayByID(clientSocket);
         break;
    case 3: // Display students by score
        displayByScore(clientSocket);
        break;
    case 4:  // Display all students
        displayAll(clientSocket);
        break;
    case 5:  // Delete student by ID
        deleteByID(clientSocket);
        break;
    case 6:  // Exit
        exit(0);
    default:
        printf("Invalid Choice\n");
        break;}}while (choice !=6);
    close(clientSocket);
    return 0;}
