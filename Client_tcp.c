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
clientSocket = socket(PF_INET, SOCK_STREAM, 0);
serverAddr.sin_family = AF_INET;
port = (unsigned short)atoi(argv[2]);
serverAddr.sin_port = htons(port);
hostnm = gethostbyname(argv[1]);
serverAddr.sin_addr.s_addr = *((unsigned long *)hostnm->h_addr);
memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
addr_size = sizeof serverAddr;
connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);
    int choice;
    do {printf("1. Add student\n2. Display student by ID\n3. Display students by score\n4. Display all students\n5. Delete student by ID\n6. Exit\n");
        // Send the choice to the server
        uint32_t network_choice = htonl(choice);
        send(clientSocket, &network_choice, sizeof(network_choice), 0);
        
        if (choice == 1) {
            // Ask for ID
            printf("Enter student ID: ");
            scanf("%s", buffer);
            send(clientSocket, buffer, strlen(buffer), 0);

            // Ask for first name
            printf("Enter first name: ");
            scanf("%s", buffer);
            send(clientSocket, buffer, strlen(buffer), 0);

            // Ask for last name
            printf("Enter last name: ");
            scanf("%s", buffer);
            send(clientSocket, buffer, strlen(buffer), 0);

            // Ask for score
            printf("Enter score: ");
            scanf("%s", buffer);
            send(clientSocket, buffer, strlen(buffer), 0);
        } else if (choice == 2 || choice == 5) {
            // Ask for ID
            printf("Enter student ID: ");
            scanf("%s", buffer);
            send(clientSocket, buffer, strlen(buffer), 0);
        } else if (choice == 3) {
            // Ask for score
            printf("Enter score: ");
            scanf("%s", buffer);
            send(clientSocket, buffer, strlen(buffer), 0);
        }  
    close(clientSocket);
    return 0;}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_BUFFER 1024

int main(int argc, char **argv){
    int clientSocket;
    char buffer[MAX_BUFFER];
    struct sockaddr_in serverAddr;
    socklen_t addr_size;
    unsigned short port;
    struct hostent *hostnm;

    clientSocket = socket(PF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    port = (unsigned short)atoi(argv[2]);
    serverAddr.sin_port = htons(port);
    hostnm = gethostbyname(argv[1]);
    serverAddr.sin_addr.s_addr = *((unsigned long *)hostnm->h_addr);
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    addr_size = sizeof serverAddr;
    connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

    int choice;
    do {
        printf("1. Add new student information to the database\n");
        printf("2. Display a student's information\n");
        printf("3. Display information of all students with a score above a certain score\n");
        printf("4. Display the information of all the students\n");
        printf("5. Delete a student entry from the database\n");
        printf("6. Exit the program\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        uint32_t network_choice = htonl(choice);
        send(clientSocket, &network_choice, sizeof(network_choice), 0);

        if (choice == 1) {
            // Ask for ID
            printf("Enter student ID: ");
            scanf("%s", buffer);
            send(clientSocket, buffer, strlen(buffer), 0);

            // Ask for first name
            printf("Enter first name: ");
            scanf("%s", buffer);
            send(clientSocket, buffer, strlen(buffer), 0);

            // Ask for last name
            printf("Enter last name: ");
            scanf("%s", buffer);
            send(clientSocket, buffer, strlen(buffer), 0);

            // Ask for score
            printf("Enter score: ");
            scanf("%s", buffer);
            send(clientSocket, buffer, strlen(buffer), 0);
        } else if (choice == 2 || choice == 5) {
            // Ask for ID
            printf("Enter student ID: ");
            scanf("%s", buffer);
            send(clientSocket, buffer, strlen(buffer), 0);
        } else if (choice == 3) {
            // Ask for score
            printf("Enter score: ");
            scanf("%s", buffer);
            send(clientSocket, buffer, strlen(buffer), 0);
        }

        // Receive and print the server's response
        recv(clientSocket, buffer, MAX_BUFFER, 0);
        printf("%s\n", buffer);

    } while (choice != 6);

    close(clientSocket);
    return 0;
}

switch (choice) {
            case 1:
                add(clientSocket);
                break;
            case 2:
                display(clientSocket);
                break;
            case 3:
                display_score(clientSocket);
                break;
            case 4:
                display_all(clientSocket);
                break;
            case 5:
                delete(clientSocket);
                break; }} while (choice != 6);

#define MAX_STUDENTS 100

struct Student {
    int ID;
    char Fname[10];
    char Lname[10];
    float score; };

struct Student students[MAX_STUDENTS];
int numStudents = 0; 

void add(int socket) {
    if (numStudents >= MAX_STUDENTS) {
        printf("Database is full.\n");
        return;
    }

    struct Student newStudent;
    printf("Enter student ID: ");
    scanf("%d", &newStudent.ID);
    printf("Enter first name: ");
    scanf("%s", newStudent.Fname);
    printf("Enter last name: ");
    scanf("%s", newStudent.Lname);
    printf("Enter score: ");
    scanf("%f", &newStudent.score);

    students[numStudents] = newStudent;
    numStudents++;
    printf("Student added successfully!\n");
}

// Function to display a student's information by ID
void display(int socket) {
    int searchID;
    printf("Enter student ID to display: ");
    scanf("%d", &searchID);

    for (int i = 0; i < numStudents; i++) {
        if (students[i].ID == searchID) {
            printf("Student ID: %d\n", students[i].ID);
            printf("Name: %s %s\n", students[i].Fname, students[i].Lname);
            printf("Score: %.2f\n", students[i].score);
            return;
        }
    }
    printf("Student with ID %d not found.\n", searchID);
}

// Function to display information of students with scores above a given value
void display_score(int socket) {
    float minScore;
    printf("Enter minimum score: ");
    scanf("%f", &minScore);

    printf("Students with scores above %.2f:\n", minScore);
    for (int i = 0; i < numStudents; i++) {
        if (students[i].score > minScore) {
            printf("ID: %d, Name: %s %s, Score: %.2f\n",
                   students[i].ID, students[i].Fname, students[i].Lname, students[i].score);
        }
    }
}

// Function to display information of all students
void display_all(int socket) {
    printf("All students in the database:\n");
    for (int i = 0; i < numStudents; i++) {
        printf("ID: %d, Name: %s %s, Score: %.2f\n",
               students[i].ID, students[i].Fname, students[i].Lname, students[i].score);
    }
}

// Function to delete a student by ID
void delete(int socket) {
    int deleteID;
    printf("Enter student ID to delete: ");
    scanf("%d", &deleteID);

    for (int i = 0; i < numStudents; i++) {
        if (students[i].ID == deleteID) {
            // Shift remaining elements to fill the gap
            for (int j = i; j < numStudents - 1; j++) {
                students[j] = students[j + 1];
            }
            numStudents--;
            printf("Student with ID %d deleted successfully!\n", deleteID);
            return;
        }
    }
    printf("Student with ID %d not found.\n", deleteID);
}




