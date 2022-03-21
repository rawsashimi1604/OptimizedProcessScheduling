#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
    ROUND ROBIN ALGORITHM
*/

#define PROCESS_COUNT 5                             // Number of processes to schedule for algorithm
#define BUFFER_LEN 255                              // Size of buffer to read line from file

// Information storage
int processArrivalTime[PROCESS_COUNT];              // Store information on process arrival time.
int processBurstTime[PROCESS_COUNT];                // Store information on process burst time.
int processFinishTime[PROCESS_COUNT];               // Store information on process finish time.
int processTurnaroundTime[PROCESS_COUNT];           // Store information on process turnaround time.
int processWaitingTime[PROCESS_COUNT];              // Store information on process waiting time.

int timeQuantum = 1;                                // Time Quantum before context switching.

// Parameters to be optimized.
float averageTurnaroundTime = 0;      
float maxTurnaroundTime = 0;           
float averageWaitingTime = 0;
float maxWaitingTime = 0;

/**
  * Get index of first occurence of char c in string. 
  * Returns index if found, -1 if not found.
*/
int getIndex(char* string, char c) {
    for (int i = 0; i < strlen(string); i++) {
        if (string[i] == c) {
            return i;
        }
    }
    return -1;
}

/**
  * Get process arrival time from file,
*/
int getArrivalTime(char* buffer) {

    char str[BUFFER_LEN];               // String to store temp buffer before converting to int.
    memset(str, '\0', BUFFER_LEN);      // Reset string (Safe)

    int i;
    for (i = 0; i < BUFFER_LEN; i++) {  // Copy contents up to ' ' character
        if (buffer[i] != ' ') {
            str[i] = buffer[i];
        } else {
            break;
        }
    }
    str[i] = '\0';                      // Append NULL character to signify end of string.

    return atoi(str);
}

/**
  * Get process burst time from file.
*/
int getBurstTime(char* buffer) {

    char str[BUFFER_LEN];               // String to store temp buffer before converting to int.
    memset(str, '\0', BUFFER_LEN);      // Reset string (Safe)

    int index = getIndex(buffer, ' ');
    strcpy(str, buffer + index);

    return atoi(str);
}

/**
  * Read input file into information storage arrays.
*/
void readInputFile(const char* filePath) {

    // Open file, if error, exit program.
    FILE* fp;
    fp = fopen(filePath, "r");

    if (fp == NULL) {                               // Error handling if file open fails.
        perror("Failed to open file..\n");
        exit(EXIT_FAILURE);
    }

    printf("\nReading <%s> now.\n\n---\n\n", filePath);

    char buffer[BUFFER_LEN];                        // Buffer to store contents read from file.

    
    // Read each line iteratively to buffer
    int p = 0;                                      // tmp variable to iterate through process.

    printf("Reading values into information storage arrays.\n\n");

    while (fgets(buffer, BUFFER_LEN, fp)) {
        // printf("%s", buffer);
        
        int arrivalTime = getArrivalTime(buffer);
        int burstTime = getBurstTime(buffer);

        printf("Process %d: ArrivalTime = %d, BurstTime = %d\n", p+1, arrivalTime, burstTime);
        p++;
    }

    printf("\nFile reading complete...\nInformation storage arrays updated...\n\n---");

    // Close file
    fclose(fp);             
    printf("\nFile closed.\n\n");             
}

/**
  * Executes the default round robin algorithm.   
*/
void defaultRoundRobin() {

    readInputFile("input.txt");

    // Output to stdout
    printf("---\nPrinting algorithm statistics...\n\n");
    printf("average turnaround time: %.2f\n", averageTurnaroundTime);
    printf("maximum turnaround time: %.2f\n", maxTurnaroundTime);
    printf("average waiting time: %.2f\n", averageWaitingTime);
    printf("maximum waiting time: %.2f\n", maxWaitingTime);
}

int main ()
{
    defaultRoundRobin();
    return 0;
} 
