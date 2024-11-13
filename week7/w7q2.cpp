#include <mpi.h>
#include <stdio.h>
#include <string.h>

#define MAX_MESSAGE_SIZE 100

int main(int argc, char *argv[]) {
    int rank, size;
    char message[MAX_MESSAGE_SIZE];

    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    // Get the number of processes and the rank of the current process
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Ensure we have exactly two processes
    if (size != 2) {
        if (rank == 0) {
            printf("This program requires exactly 2 processes.\n");
        }
        MPI_Finalize();
        return 1;
    }

    if (rank == 0) {
        // Process 0: Send a greeting message to Process 1
        strcpy(message, "Hello from Process 0!");
        printf("Process 0: Sending message to Process 1: %s\n", message);
        MPI_Send(message, strlen(message) + 1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);

        // Process 0: Receive the modified message from Process 1
        MPI_Recv(message, MAX_MESSAGE_SIZE, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 0: Received modified message from Process 1: %s\n", message);
    } else if (rank == 1) {
        // Process 1: Receive the message from Process 0
        MPI_Recv(message, MAX_MESSAGE_SIZE, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1: Received message from Process 0: %s\n", message);

        // Process 1: Modify the message (Ensure no buffer overflow)
        if (strlen(message) + strlen(" And hello back from Process 1!") < MAX_MESSAGE_SIZE) {
            strcat(message, " And hello back from Process 1!");
            printf("Process 1: Modified message: %s\n", message);
        } else {
            printf("Process 1: Not enough space to modify the message safely.\n");
        }

        // Process 1: Send the modified message back to Process 0
        MPI_Send(message, strlen(message) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }

    // Finalize the MPI environment
    MPI_Finalize();

    return 0;
}

