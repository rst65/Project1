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
uint32_t choice;
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


    while(1) {

            addr_size = sizeof serverStorage;
newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage,
&addr_size);
            // Handle the commands from the client
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
                        break;
                }
            } while (choice != 6);

            close(newSocket);
            exit(0);
        }

        // Parent process closes the new socket and goes back to listening
       close(newSocket);
close(welcomeSocket);


    return 0;}
