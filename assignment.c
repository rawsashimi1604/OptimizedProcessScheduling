#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
    OPTIMIZED ROUND ROBIN ALGORITHM
*/

#define MAX_PROCESS_COUNT 99                        // Maximum number of processes to schedule for algorithm
#define BUFFER_LEN 255                              // Size of buffer to read line from file
                         

int processCount = 0;                               // Number of processes read. 

// Information storage
int processArrivalTime[MAX_PROCESS_COUNT];          // Store information on process arrival time.
int processBurstTime[MAX_PROCESS_COUNT];            // Store information on process burst time.
int processBackupBurstTime[MAX_PROCESS_COUNT];      // Store information on process burst time. (backup)
int processFinishTime[MAX_PROCESS_COUNT];           // Store information on process finish time.
int processResponseTime[MAX_PROCESS_COUNT];         // Store information on process response time.
int processTurnaroundTime[MAX_PROCESS_COUNT];       // Store information on process turnaround time.
int processWaitingTime[MAX_PROCESS_COUNT];          // Store information on process waiting time.

int timeQuantum = 1;                                // Time Quantum before context switching.

// Parameters to be optimized.
float averageTurnaroundTime = 0;      
float maxTurnaroundTime = 0;           
float averageWaitingTime = 0;
float maxWaitingTime = 0;

// Queue Data Structure
typedef struct {
    int inp_arr[MAX_PROCESS_COUNT];
    int Rear; 
    int Front;
} Queue;


void enqueue();
void dequeue();
void show();

Queue* createQueue() {
    Queue* q = malloc(sizeof(Queue));
    q->Front = -1;
    q->Rear = -1;

    return q;
}

void enqueue(Queue* q, int n)
{
    int insert_item = n;
    if (q->Rear == MAX_PROCESS_COUNT - 1)
       printf("Overflow \n");
    else
    {
        if (q->Front == - 1)
       
        q->Front = 0;
        q->Rear = q->Rear + 1;
        q->inp_arr[q->Rear] = insert_item;
    }
} 
  
void dequeue(Queue* q)
{
    if (q->Front == - 1 || q->Front > q->Rear)
    {
        printf("Underflow \n");
        return ;
    }
    else
    {
        q->Front = q->Front + 1;
    }
} 
  
void show(Queue* q)
{
    if (q->Front == - 1)
        printf("Empty Queue \n");
    else
    {   
        printf("Queue: \n");
        printf("[");
        for (int i = q->Front; i <= q->Rear; i++)
            printf("%d, ", q->inp_arr[i]);
        printf("]");
        printf("\n");
    }
} 

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

        processArrivalTime[p] = arrivalTime;
        processBurstTime[p] = burstTime;
        processBackupBurstTime[p] = burstTime;

        printf("Process %d: ArrivalTime = %d, BurstTime = %d\n", p+1, arrivalTime, burstTime);
        p++;
        processCount++;
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
    /*
        Read input file into information storage arrays.
    */
    readInputFile("input.txt");                    

    /*
        PERFORM ALGORITHM.
    */
    
    // Get maximum process burst time.
    int maxBurstTime = processBurstTime[0];         
    int i;

    for (i = 1; i < processCount; i++){ 
        if (maxBurstTime < processBurstTime[i]) {
            maxBurstTime = processBurstTime[i];
        }
    }

    // simulation of round robin process
    Queue* queue = createQueue();                   // Create Queue data structure

    int currTime = 0; int currProcess = -1;
    while (true) {
        
        // If process arrives, add to queue
        if (processArrivalTime[i] >= currTime) {

        }
        break;
    }


    // Get finish time
    for (i = 0; i < processCount; i++) {
        processFinishTime[i] = processTurnaroundTime[i] + processArrivalTime[i];
    }

    printf("\n\t PROCESS\t ARRIVAL TIME\t BURST TIME\t FINISH TIME\t TURNAROUND TIME\t WAITING TIME\t RESPONSE TIME\n");
    for (i = 0; i < processCount; i++) {
        printf("\t P%d \t\t %d \t\t %d \t\t %d \t\t %d \t\t\t %d\t\t %d \n",
            i + 1, processArrivalTime[i], processBackupBurstTime[i], processFinishTime[i], processTurnaroundTime[i], processWaitingTime[i], processResponseTime[i]);
    }

    /*
        Output algorithm statistics
    */
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
