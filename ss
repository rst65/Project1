#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define MAX_STUDENTS 100
#define MAX_BUFFER 1024

int serverSocket, newSocket;

struct Student {
    int ID;
    char Fname[50];
    char Lname[50];
    float score;};

struct Student students[MAX_STUDENTS];
int numStudents = 0;

void add(int socket) {
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

void display(int socket) {
    char buffer[1024];

    // Ask for ID
    char *prompt = "Enter student ID to display: ";
    send(newSocket, prompt, strlen(prompt), 0);
    recv(newSocket, buffer, 1024, 0);

    int searchID;
    sscanf(buffer, "%d", &searchID);

    for (int i = 0; i < numStudents; i++) {
        if (students[i].ID == searchID) {
            sprintf(buffer, "Student ID: %d\nName: %s %s\nScore: %.2f\n", students[i].ID, students[i].Fname, students[i].Lname, students[i].score);
            send(newSocket, buffer, strlen(buffer), 0);
            return;} }
    char *message = "Student not found.\n";
    send(newSocket, message, strlen(message), 0);}

void display_score(int socket) {
    char buffer[1024];

    // Ask for minimum score
    char *prompt = "Enter minimum score: ";
    send(newSocket, prompt, strlen(prompt), 0);
    recv(newSocket, buffer, 1024, 0);

    float minScore;
    sscanf(buffer, "%f", &minScore);

    for (int i = 0; i < numStudents; i++) {
        if (students[i].score > minScore) {
            sprintf(buffer, "ID: %d, Name: %s %s, Score: %.2f\n", students[i].ID, students[i].Fname, students[i].Lname, students[i].score);
            send(newSocket, buffer, strlen(buffer), 0); } }}

void display_all(int socket) {
    char buffer[1024];

    for (int i = 0; i < numStudents; i++) {
        sprintf(buffer, "ID: %d, Name: %s %s, Score: %.2f\n", students[i].ID, students[i].Fname, students[i].Lname, students[i].score);
        send(newSocket, buffer, strlen(buffer), 0); }}

void delete(int socket) {
    char buffer[1024];

    for (int i = 0; i < numStudents; i++) {
        sprintf(buffer, "ID: %d, Name: %s %s, Score: %.2f\n", students[i].ID, students[i].Fname, students[i].Lname, students[i].score);
        send(newSocket, buffer, strlen(buffer), 0); }}

int main(int argc, char **argv){
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

    serverSocket = socket(PF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12000);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    if(listen(serverSocket,50)==0)
        printf("Listening\n");
    else
        printf("Error\n");

    while (1){
    addr_size = sizeof serverStorage;
    newSocket = accept(serverSocket, (struct sockaddr *) &serverStorage, &addr_size);

    int choice;
    do {
        recv(newSocket, &choice, sizeof(choice), 0);
                choice = ntohl(choice);

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
                break;}} while (choice != 6);
     close(newSocket); }

                   close(newSocket);
close(serverSocket);
    return 0;}
~
