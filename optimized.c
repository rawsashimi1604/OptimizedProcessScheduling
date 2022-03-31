#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
    OPTIMIZED ROUND ROBIN ALGORITHM
*/

#define MAX_PROCESS_COUNT 99                        // Maximum number of processes to schedule for algorithm
#define BUFFER_LEN 255                              // Size of buffer to read line from file

// ---------------------------------------
//  Struct Declarations
// ---------------------------------------

typedef struct {
    int processNumber;
    int remainingTime;
    bool started;

    int arrivalTime;
    int startTime;
    int burstTime;
    int finishTime;
} Process;

typedef struct {
    Process* inp_arr[MAX_PROCESS_COUNT];
    int Rear; 
    int Front;
    int size;
} Queue;

// ---------------------------------------
//  Function Declarations
// ---------------------------------------

// Process Functions
void printProcess(Process* p);

// Queue Functions
Queue* createQueue();
void enqueue(Queue* q, Process* p);
Process* peek(Queue* q);
Process* dequeue(Queue* q);
void sortQueue(Queue* q);
void show();
int calculateDynamicTQ(Queue* q);


// ---------------------------------------
//  Global Variables
// ---------------------------------------

int processCount = 0;                               // Number of processes read. 

// Information storage
int processArrivalTime[MAX_PROCESS_COUNT];          // Store information on process arrival time.
int processBurstTime[MAX_PROCESS_COUNT];            // Store information on process burst time.
int processBackupBurstTime[MAX_PROCESS_COUNT];      // Store information on process burst time. (backup)
int processFinishTime[MAX_PROCESS_COUNT];           // Store information on process finish time.
int processResponseTime[MAX_PROCESS_COUNT];         // Store information on process response time.
int processTurnaroundTime[MAX_PROCESS_COUNT];       // Store information on process turnaround time.
int processWaitingTime[MAX_PROCESS_COUNT];          // Store information on process waiting time.
bool processAdded[MAX_PROCESS_COUNT];               // Store information on whether process has been added to queue.

int timeQuantum = 1;                                // Time Quantum before context switching.

// Parameters to be optimized.
float averageTurnaroundTime = 0;      
float maxTurnaroundTime = 0;           
float averageWaitingTime = 0;
float maxWaitingTime = 0;

// ---------------------------------------
//  Process Functions
// ---------------------------------------

void printProcess(Process* p) {
    printf("(P%d, %d)\n", p->processNumber, p->remainingTime);
}

// ---------------------------------------
//  Queue Functions
// ---------------------------------------

/**
  * Dynamically allocates memory for a Queue.
  * Returns pointer to the dynamically allocated Queue.
*/
Queue* createQueue() {
    Queue* q = malloc(sizeof(Queue));
    q->Front = -1;
    q->Rear = -1;
    q->size = 0;
    return q;
}

/**
  * Adds a Process to a Queue.
*/
void enqueue(Queue* q, Process* p)
{
    Process* insert_item = p;
    if (q->Rear == MAX_PROCESS_COUNT - 1)
       printf("Overflow \n");
    else
    {
        if (q->Front == - 1)
       
        q->Front = 0;
        q->Rear = q->Rear + 1;
        q->inp_arr[q->Rear] = insert_item;
        q->size = q->size + 1;
    }
} 

/**
  * Removes a Process from a Queue. 
  * Returns pointer to the Process removed.
*/
Process* dequeue(Queue* q)
{
    if (q->Front == - 1 || q->Front > q->Rear)
    {
        printf("Underflow \n");
        return NULL;
    }
    else
    {   
        Process* tmp = q->inp_arr[q->Front];
        q->Front = q->Front + 1;
        q->size = q->size - 1;
        return tmp;
    }
} 

/**
  * Displays all Processes (Process Number, Remaining Time) in the Queue.
*/
void show(Queue* q)
{
    if (q->Front == - 1)
        printf("Empty Queue \n");
    else
    {   
        printf("Queue: ");
        printf("[");
        for (int i = q->Front; i <= q->Rear; i++)
            printf("(P%d, %d), ", q->inp_arr[i]->processNumber, q->inp_arr[i]->remainingTime);
        printf("]");
        printf("\n");
    }
} 

/**
  * Returns true if Queue has 1 or more Processes in it. False otherwise.
*/
int isEmpty(Queue* q) {
    return q->size == 0;
}

/**
  * Returns the pointer to the Process at the front of the Queue.
*/
Process* peek(Queue* q){
    if (isEmpty(q)) {
        return NULL;
    }

    Process* tmp = q->inp_arr[q->Front];
    return tmp;
}

/**
  * Sorts the Queue, placing the process with the least remaining time at the start of the Queue.
*/
void sortQueue(Queue* q) {

    Process* tmp[MAX_PROCESS_COUNT];    // Temporary array to store Processes

    int n = 0;                          // Number of elements in Queue
    int size = q->size;

    // Deqeueue all elements to a temporary array.
    while(!isEmpty(q)) {
        tmp[n] = dequeue(q);
        n++;
    }
    
    // Insertion sort, by remaining time.
    for (int i = 1; i < size; i++) {
        Process* currProcess = tmp[i];
        int pos = i;

        while (pos > 0 && tmp[pos-1]->remainingTime > currProcess->remainingTime) {
            tmp[pos] = tmp[pos-1];
            pos = pos-1;
        }

        tmp[pos] = currProcess;
    }

    // Add back to queue
    for (int i = 0; i < size; i++) {
        enqueue(q, tmp[i]);
    }

    printf("Sorted queue!\n"); show(q);
}

/**
  * Calculates Time Quantum value based on average remaining time amoung Processes in Queue.
*/
int calculateDynamicTQ(Queue* q) {
    int sum = 0;

    Process* tmp[MAX_PROCESS_COUNT];

    int n = 0;                          // Number of elements in Queue
    int size = q->size;

    // Deqeueue all elements to a temporary array.
    while(!isEmpty(q)) {
        tmp[n] = dequeue(q);
        n++;
    }

    for (int i = 0; i < size; i++) {
        sum += tmp[i]->remainingTime;
    }

    // Add back to queue
    for (int i = 0; i < size; i++) {
        enqueue(q, tmp[i]);
    }

    return sum / q->size;
}

// ---------------------------------------
//  Ohter Functions
// ---------------------------------------

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
  * Executes the optimized round robin algorithm.   
*/
void optimizedRoundRobin() {
    /*
        Read input file into information storage arrays.
    */
    readInputFile("input.txt");                    

    /*
        PERFORM ALGORITHM.
    */
    // Set all processAdded elements to false
    for (int i = 0; i < processCount; i++) {
        processAdded[i] = false;
    }


    // Get maximum process burst time.
    int maxBurstTime = processBurstTime[0];         
    int i;

    for (i = 1; i < processCount; i++){ 
        if (maxBurstTime < processBurstTime[i]) {
            maxBurstTime = processBurstTime[i];
        }
    }

    // simulation of round robin process
    Queue* queue = createQueue();                   // Create Queue
    Process* cpu = NULL;                            // READY process to be executed

    int count = 0;                                  // Count of processes completed. If all processes are completed, break the algorithm.
    int currTime = 0;                               // Keep track of current time.
    int tmpQuantum = 0;                             // Keeping track when to switch.
    bool switchFlag = false;                        // Keeping track of time quantum switch.

    int cycleCount = 0;                             // Current process cycle count.
    int cycleSize = 0;                              // Current process cycle size.
    bool algoCycleFlag = true;                      // Keeping track of algorithm cycling, only update time quantum when cycle has completed.

    // round robin algo 
    // with dynamic time quantum that changes based on the avg burst time of process in the ready queue
    // process with least burst time will be executed first
    // After all processes run once, we re-calculate the dynamic time quantum.
    // This is repeated until all processes are completed.

    printf("ALGORITHM\n---\n");
    while (true) {
        if (cpu) {
            printf("CPU "); printProcess(cpu);
        } else {
            printf("CPU: NULL\n");
        }
        
        // Add processes to Queue
        for (int i = 0; i < processCount; i++) {
            // If process arrives, add to queue. If already added, do not add again...
            if (currTime >= processArrivalTime[i] && !processAdded[i]) {
                Process* p = malloc(sizeof(Process));

                // Add process parameters
                p->processNumber = i;
                p->arrivalTime = processArrivalTime[i];
                p->burstTime = processBurstTime[i];        
                p->remainingTime = processBurstTime[i];
                enqueue(queue, p);

                processAdded[i] = true;

                printf("Added process %d to Queue!\n", p->processNumber);
            }
        }

        printf("Queue BEFORE EXECUTING "); show(queue);

        // If cpu has a process, execute
        if (cpu) {
            
            cpu->remainingTime = cpu->remainingTime - 1;                    // Execute process
            printf("Executing process %d now\n", cpu->processNumber);
            printf("Current Status: "); printProcess(cpu);
            tmpQuantum++;                                                   // Keep track of time quantum
            printf("tmpQuantum = %d\n", tmpQuantum);
            if (!cpu->started) {                                            // Keep track of CPU Start Time. (Used in calculations later)
                cpu->startTime = currTime-1;
                cpu->started = true;
            }

            // Process has finished executing
            if (cpu->remainingTime <= 0) {  

                cycleCount++;                                               // Update cycle count. (tracking when to recalculate DTQ)

                cpu->finishTime = currTime;
                printf("P%d has finished executing at time %d!!!!\n", cpu->processNumber, cpu->finishTime);

                // Update information arrays...
                processFinishTime[cpu->processNumber] = cpu->finishTime;
                processTurnaroundTime[cpu->processNumber] = cpu->finishTime - cpu->arrivalTime;
                processWaitingTime[cpu->processNumber] = processTurnaroundTime[cpu->processNumber] - cpu->burstTime;
                processResponseTime[cpu->processNumber] = cpu->startTime - cpu->arrivalTime;

                free(cpu);
                cpu = NULL;
            
                count++;

                if (count == processCount) {
                    break;
                }
            }

            // Time quantum exceeded, add back to queue. 
            else if (tmpQuantum >= timeQuantum) {
                cycleCount++;                                               // Update cycle count. (tracking when to recalculate DTQ)
                enqueue(queue, cpu);
                printf("Switch Process %d out.\n", cpu->processNumber);
                switchFlag = true;
                tmpQuantum = 0;
            }
        }

        printf("Queue AFTER EXECUTING "); show(queue);

        // If CPU is empty, ready next process to be executed...
        if (cpu == NULL) {
            switchFlag = true;
            tmpQuantum = 0;
        }

        // Track when the round robin cycle has completed.
        if (cycleCount == cycleSize) {
            algoCycleFlag = true;
        }

        // If queue is not empty, dequeue and put into CPU.
        if (!isEmpty(queue) && switchFlag) {

            // If new cycle has begun, calculate dynamic timeQuantum
            if (algoCycleFlag) {
                timeQuantum = calculateDynamicTQ(queue);
                cycleCount = 0;                             // Reset cycle count.
                cycleSize = queue->size;                    // Reset cycle size.
                algoCycleFlag = false;

                sortQueue(queue); 

                printf("Updated TIME QUANTUM to %d!\n", timeQuantum);
            }

            // Add new process to CPU from front of Queue
            Process* tmp = dequeue(queue);
            cpu = tmp;

            // Else, the current process in CPU remains
            printf("Process %d added to CPU\n", cpu->processNumber);   

            
        }

        if (cpu) {
            printf("Next Process: "); printProcess(cpu); 
        } else {
            printf("Next Process: NULL"); 
        }
        

        printf("count = %d, currTime = %d, timeElapsed = %d\n---\n", count, currTime, currTime+1);
        
        currTime++;             // Update time.
        switchFlag = false;     // Reset switch flag.

        

    }

    free(queue);                // Free memory dynamically allocated to Queue.

    /*
        Output algorithm statistics
    */

    // Print statistics heaaders.
    printf("\n\t PROCESS\t ARRIVAL TIME\t BURST TIME\t FINISH TIME\t TURNAROUND TIME\t WAITING TIME\t RESPONSE TIME\n");
    for (i = 0; i < processCount; i++) {
        printf("\t P%d \t\t %d \t\t %d \t\t %d \t\t %d \t\t\t %d\t\t %d \n",
            i, processArrivalTime[i], processBackupBurstTime[i], processFinishTime[i], processTurnaroundTime[i], processWaitingTime[i], processResponseTime[i]);
    }

    // Calculate statistics
    maxTurnaroundTime = processTurnaroundTime[0];
    float tmpTurnaroundTime = 0;
    for (int i = 0; i < processCount; i++) {
        tmpTurnaroundTime += processTurnaroundTime[i];
        if (processTurnaroundTime[i] > maxTurnaroundTime) {
            maxTurnaroundTime = processTurnaroundTime[i];
        } 
    }
    averageTurnaroundTime = tmpTurnaroundTime / processCount;

    maxWaitingTime = processWaitingTime[0];
    float tmpWaitingTime = 0;
    for (int i = 0; i < processCount; i++) {
        tmpWaitingTime += processWaitingTime[i];
        if (processWaitingTime[i] > maxWaitingTime) {
            maxWaitingTime = processWaitingTime[i];
        } 
    }
    averageWaitingTime = tmpWaitingTime / processCount;

    
    printf("---\nPrinting algorithm statistics...\n\n");
    printf("average turnaround time: %.2f\n", averageTurnaroundTime);
    printf("maximum turnaround time: %.2f\n", maxTurnaroundTime);
    printf("average waiting time: %.2f\n", averageWaitingTime);
    printf("maximum waiting time: %.2f\n", maxWaitingTime);
    printf("time quantum: %d\n", timeQuantum);
    
    // output to txt file
    FILE *fptr;
    fptr = fopen("optimized_output.txt" , "a");
    fprintf(fptr,"%.2f %.2f %.2f %.2f\n", averageTurnaroundTime, maxTurnaroundTime, averageWaitingTime, maxWaitingTime);
    fclose(fptr);
}

int main ()
{
    optimizedRoundRobin();
    return 0;
} 
