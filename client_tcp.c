#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>

struct Student {
    int ID;
    char Fname[50];
    char Lname[50];
    float score;
};

// Function to add a new student to the database
void add(struct Student students[], int *numStudents) {
    if (*numStudents >= 100) {
        printf("Database is full. Cannot add more students.\n");
        return;
    }

    printf("Enter student ID: ");
    scanf("%d", &students[*numStudents].ID);
    printf("Enter first name: ");
    scanf("%s", students[*numStudents].Fname);
    printf("Enter last name: ");
    scanf("%s", students[*numStudents].Lname);
    printf("Enter score: ");
    scanf("%f", &students[*numStudents].score);

    (*numStudents)++;
}

// Function to display student information by ID
void displayByID(struct Student students[], int numStudents, int ID) {
    for (int i = 0; i < numStudents; i++) {
        if (students[i].ID == ID) {
            printf("Student ID: %d\n", students[i].ID);
            printf("Name: %s %s\n", students[i].Fname, students[i].Lname);
            printf("Score: %.2f\n", students[i].score);
            return;
        }
    }
    printf("Student with ID %d not found.\n", ID);
}

// Function to display students with scores above a given value
void displayByScore(struct Student students[], int numStudents, float score) {
    printf("Students with scores above %.2f:\n", score);
    for (int i = 0; i < numStudents; i++) {
        if (students[i].score > score) {
            printf("ID: %d, Name: %s %s, Score: %.2f\n",
                   students[i].ID, students[i].Fname, students[i].Lname, students[i].score);
        }
    }
}

// Function to display all students
void displayAll(struct Student students[], int numStudents) {
    printf("All students in the database:\n");
    for (int i = 0; i < numStudents; i++) {
        printf("ID: %d, Name: %s %s, Score: %.2f\n",
               students[i].ID, students[i].Fname, students[i].Lname, students[i].score);
    }
}

// Function to delete a student by ID
void deleteByID(struct Student students[], int *numStudents, int ID) {
    for (int i = 0; i < *numStudents; i++) {
        if (students[i].ID == ID) {
            // Shift remaining students to fill the gap
            for (int j = i; j < *numStudents - 1; j++) {
                students[j] = students[j + 1];
            }
            (*numStudents)--;
            printf("Student with ID %d deleted.\n", ID);
            return;
        }
    }
    printf("Student with ID %d not found.\n", ID);
}

int main(int argc, char **argv){
int clientSocket;
char buffer[1024];
struct sockaddr_in serverAddr;
socklen_t addr_size;
unsigned short port;
struct hostent *hostnm;
uint32_t choice, cnum;
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
// send an integer to the server

    struct Student students[100];
    int numStudents = 0;
    int choice, ID;
    float score;
    // Other necessary variables (e.g., database storage)
while (1) {
        printf("\nMenu:\n");
        printf("1. Add student\n");
        printf("2. Display student by ID\n");
        printf("3. Display students by score\n");
        printf("4. Display all students\n");
        printf("5. Delete student by ID\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                add(students, &numStudents);
                break;
            case 2:
                printf("Enter student ID: ");
                scanf("%d", &ID);
                displayByID(students, numStudents, ID);
                break;
            case 3:
                printf("Enter score: ");
                scanf("%f", &score);
                displayByScore(students, numStudents, score);
                break;
            case 4:
                displayAll(students, numStudents);
                break;
            case 5:
                printf("Enter student ID to delete: ");
                scanf("%d", &ID);
                deleteByID(students, &numStudents, ID);
                break;
            case 6:
                printf("Exiting program.\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
 

typedef struct {
    int ID;
    char Fname[50];
    char Lname[50];
    float score[];
} Student;

Student students[100];
int student_count = 0;

void add(int ID, char* Fname, char* Lname, float score) {
    students[student_count].ID = ID;
    strcpy(students[student_count].Fname, Fname);
    strcpy(students[student_count].Lname, Lname);
    students[student_count].score = score;
    student_count++;
}

void display_by_ID(int ID) {
    for(int i = 0; i < student_count; i++) {
        if(students[i].ID == ID) {
            printf("ID: %d, Name: %s %s, Score: %.2f\n", students[i].ID, students[i].Fname, students[i].Lname, students[i].score);
            return;
        }
    }
    printf("No student with ID: %d\n", ID);
}

void display_by_score(float score) {
    for(int i = 0; i < student_count; i++) {
        if(students[i].score > score) {
            printf("ID: %d, Name: %s %s, Score: %.2f\n", students[i].ID, students[i].Fname, students[i].Lname, students[i].score);
        }
    }
}

void display_all() {
    for(int i = 0; i < student_count; i++) {
        printf("ID: %d, Name: %s %s, Score: %.2f\n", students[i].ID, students[i].Fname, students[i].Lname, students[i].score);
    }
}

void delete_by_ID(int ID) {
    int index = -1;
    for(int i = 0; i < student_count; i++) {
        if(students[i].ID == ID) {
            index = i;
            break;
        }
    }
    if(index != -1) {
        for(int i = index; i < student_count - 1; i++) {
            students[i] = students[i + 1];
        }
        student_count--;
    } else {
        printf("No student with ID: %d\n", ID);
    }
}