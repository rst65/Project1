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




int main(int argc, char **argv){
int clientSocket;
char buffer[1024];
struct sockaddr_in serverAddr;
socklen_t addr_size;
unsigned short port;
struct hostent *hostnm;
uint32_t cnum;
char msg[30];
/*---- Create the socket. The three arguments are: ----*/
/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
clientSocket = socket(PF_INET, SOCK_STREAM, 0);
/*---- Configure settings of the server address struct ----*/
/* Address family = Internet */
serverAddr.sin_family = AF_INET;
/* set port number*/
port = (unsigned short)atoi(argv[2]);
/* htons() stands for "host to network short" and is used to convert the port
number stored in the "port" variable from host byte order to network byte order,
ensuring that data is not misinterpreted when sent over the network. */
serverAddr.sin_port = htons(port);
/* Set IP address to localhost */
hostnm = gethostbyname(argv[1]);
/* This sets the serverAddr structure's sin_addr member to the host address
provided by the hostnm->h_addr variable. The s_addr field contains the IP address
of the host in network byte order, and the *((unsigned long *)hostnm->h_addr)
expression casts the data to an unsigned long type. This ensures that the IP
address taken from hostnm is formatted correctly for network communication. */
serverAddr.sin_addr.s_addr = *((unsigned long *)hostnm->h_addr);
/* Set all bits of the padding field to 0. It is used to ensure that the serverAddr
structure is correctly zero initialized before use, which is necessary for certain
network operations. */
memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
/*---- Connect the socket to the server using the address struct ----*/
addr_size = sizeof serverAddr;
connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);
    // communication starts from here
    // Display the menu and get the user's choice
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

        // Send the choice to the server
        uint32_t network_choice = htonl(choice);
        send(clientSocket, &network_choice, sizeof(network_choice), 0);

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
                break;
        }
    } while (choice != 6);
return 0;}


