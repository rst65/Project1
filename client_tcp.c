#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>

typedef struct {
    int ID;
    char Fname[50];
    char Lname[50];
    float score;
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

    int ID;
    char Fname[50];
    char Lname[50];
    int score;
    // Other necessary variables (e.g., database storage)

 do{
   printf("1. Add student\n2. Display student by ID\n3. Display students by score\n4. Display all students\n5. Delete student by ID\n6. Exit\n");
scanf("%d", &choice);
 switch(choice) {
            case 1:
                // Code to add student
                   printf("Enter ID, first name, last name, and score: ");
                scanf("%d %s %s %f", &ID, Fname, Lname, &score);
                add(ID, Fname, Lname, score);
                break;
            case 2:
                // Code to display student by ID
                    printf("Enter ID: ");
                scanf("%d", &ID);
                display_by_ID(ID);
                break;
            case 3:
                // Code to display students by score
                     printf("Enter score: ");
                scanf("%f", &score);
                display_by_score(score);
                break;
            case 4:
                // Code to display all students
                display_all();
                break;
            case 5:
                   printf("Enter ID: ");
                scanf("%d", &ID);
                delete_by_ID(ID);
                // Code to delete student by ID
                break;
            case 6:
                // Code to exit
                return 0;
            default:
                printf("Invalid choice. Please choose again.\n");
        }
    }while(choice!=6);
/* htonl stands for "host to network long" and is a function used in networking
applications to convert values from host byte order to network byte order.*/
cnum = htonl(choice);
send(clientSocket, &cnum, sizeof(cnum), 0);
// receive a reply message from the server
recv(clientSocket, msg, sizeof(msg), 0);
printf("%s\n", msg);
close(clientSocket);

return 0;
}
