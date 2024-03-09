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
    float score;};

struct Student students[MAX_STUDENTS];
int numStudents = 0;

void add(int newSocket) {
    char buffer[1024];
    if (numStudents >= MAX_STUDENTS) {
        char *message = "Database is full.\n";
        send(newSocket, message, strlen(message), 0);
        return;}
    struct Student newStudent;
    ssize_t len;
    char *prompt = "Enter student ID: ";
    send(newSocket, prompt, strlen(prompt), 0);
    len = recv(newSocket, buffer, 1024 - 1, 0);
    buffer[len] = '\0';
    sscanf(buffer, "%d", &newStudent.ID);
    prompt = "Enter first name: ";
    send(newSocket, prompt, strlen(prompt), 0);
    len = recv(newSocket, buffer, 1024 - 1, 0);
    buffer[len] = '\0';
    sscanf(buffer, "%s", newStudent.Fname);
    prompt = "Enter last name: ";
    send(newSocket, prompt, strlen(prompt), 0);
    len = recv(newSocket, buffer, 1024 - 1, 0);
    buffer[len] = '\0';
    sscanf(buffer, "%s", newStudent.Lname);
    prompt = "Enter score: ";
    send(newSocket, prompt, strlen(prompt), 0);
    len = recv(newSocket, buffer, 1024 - 1, 0);
    buffer[len] = '\0';
    sscanf(buffer, "%f", &newStudent.score);
    students[numStudents] = newStudent;
    numStudents++;
    FILE *file = fopen("students.txt", "a");
    if (file != NULL) {
        fprintf(file, "ID: %d, Name: %s %s, Score: %.2f\n", newStudent.ID, newStudent.Fname, newStudent.Lname, newStudent.score);
        fclose(file);}
    char *successMessage = "Student added!\n";
    send(newSocket, successMessage, strlen(successMessage), 0);}


void displayByID(int newSocket) {
    char buffer[1024];
    int ID;
    recv(newSocket, buffer, sizeof(buffer)-1, 0);
    sscanf(buffer, "%d", &ID);
    for (int i = 0; i < numStudents; i++) {
        if (students[i].ID == ID) {
           
            sprintf(buffer, "ID: %d, Name: %s %s, Score: %.2f\n", students[i].ID, students[i].Fname, students[i].Lname, students[i].score);
            send(newSocket, buffer, strlen(buffer) + 1, 0);
            return;}}
    char *errorMessage = "No student found.\n";
    send(newSocket, errorMessage, strlen(errorMessage), 0);}

void displayByScore(int newSocket) {
    char buffer[1024];
    float scoreThreshold;
    recv(newSocket, buffer, sizeof(buffer)-1, 0);
    sscanf(buffer, "%f", &scoreThreshold);
    for (int i = 0; i < numStudents; i++) {
        if (students[i].score > scoreThreshold) {
            sprintf(buffer, "ID: %d, Name: %s %s, Score: %.2f\n", students[i].ID, students[i].Fname, students[i].Lname, students[i].score);
            send(newSocket, buffer, strlen(buffer) + 1, 0);}}}

void displayAll(int newSocket) {
    char buffer[1024];
    for (int i = 0; i < numStudents; i++) {
        sprintf(buffer, "ID: %d, Name: %s %s, Score: %.2f\n", students[i].ID, students[i].Fname, students[i].Lname, students[i].score);
        send(newSocket, buffer, strlen(buffer) + 1, 0);}}

void deleteByID(int newSocket) {
    char buffer[1024];
    int ID;
    recv(newSocket, buffer, sizeof(buffer)-1, 0);
    sscanf(buffer, "%d", &ID);
    for (int i = 0; i < numStudents; i++) {
        if (students[i].ID == ID) {
         
            for (int j = i; j < numStudents - 1; j++) {
                students[j] = students[j + 1];}
          numStudents--;
            char *successMessage = "Student deleted!\n";
            send(newSocket, successMessage, strlen(successMessage), 0);
            return;}}
    char *errorMessage = "No student found\n";
    send(newSocket, errorMessage, strlen(errorMessage), 0);}

int main(int argc, char **argv){
char buffer[1024];
int welcomeSocket, newSocket;
struct sockaddr_in serverAddr;
struct sockaddr_storage serverStorage;
socklen_t addr_size;
unsigned short port;
uint32_t num;
char msg[30];
welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
serverAddr.sin_family = AF_INET;
port = (unsigned short) atoi(argv[1]);
serverAddr.sin_port = htons(port);
serverAddr.sin_addr.s_addr = INADDR_ANY;
memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
if(listen(welcomeSocket,5)==0)
printf("Listening\n");
else
printf("Error\n");
addr_size = sizeof serverStorage;

 while(1){
newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);
int choice;
recv(newSocket, &choice, sizeof(choice), 0);
choice = ntohl(choice);  
printf("Received choice: %d\n", choice);
switch (choice) {
    case 1:  // Adds new students to the database and txt file
        add(newSocket);
        break;
    case 2:  // Display student by ID
        displayByID(newSocket);
        break;
    case 3:  // Display students by score
        displayByScore(newSocket);
        break;
    case 4:  // Display all students
        displayAll(newSocket);
        break;
    case 5:  // Delete student by ID
        deleteByID(newSocket);
        break;
    case 6:  // Exit
        exit(0);
    default:
        printf("Invalid Choice\n");
        break;}}
close(newSocket);
close(welcomeSocket);
return 0;}
