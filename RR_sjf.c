/**
  * SJF and RR. Queue sort in ascending order.
  * 1st Time quantum is burst time of shortest job in queue. 
  * Subsequent time quantum will double.
*/
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
bool processAdded[MAX_PROCESS_COUNT];               // Store information on whether process has been added to queue.

int remainingTimeCount = 0;                         //total burst time of processes in queue.
int qitemCount = 0;                                 //number of processes in queue.

int timeQuantum = 1;                                // Time Quantum before context switching.

// Parameters to be optimized.
float averageTurnaroundTime = 0;      
float maxTurnaroundTime = 0;           
float averageWaitingTime = 0;
float maxWaitingTime = 0;

// Process Data Structure
typedef struct {
    int processNumber;
    int remainingTime;
    bool started;

    int arrivalTime;
    int startTime;
    int burstTime;
    int finishTime;
} Process;

void printProcess(Process* p);

void printProcess(Process* p) {
    printf("(P%d, %d)\n", p->processNumber, p->remainingTime);
}

// Queue Data Structure
typedef struct {
    Process* inp_arr[MAX_PROCESS_COUNT];
    int Rear; 
    int Front;
    int size;
} Queue;

Queue* createQueue();
void enqueue(Queue* q, Process* p);
Process* peek(Queue* q);
Process* dequeue(Queue* q);
int show();

Queue* createQueue() {
    Queue* q = malloc(sizeof(Queue));
    q->Front = -1;
    q->Rear = -1;
    q->size = 0;
    return q;
}

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

int show(Queue* q)
{

    if (q->Front == - 1)
        printf("Empty Queue \n");
    else
    {   
        printf("Queue: ");
        printf("[");
        int remainingTimeCount = 0;
        int temp = 0;
        int proNoTemp = 0;
        for (int i = q->Front; i <= q->Rear; i++) {
            for (int j = i+1; j <= q->Rear; j++) {
                if (((q->inp_arr[i]->remainingTime) > (q->inp_arr[j]->remainingTime)) && (q->inp_arr[j]->processNumber != 0)) {
                    temp = (q->inp_arr[i]->remainingTime);
                    q->inp_arr[i]->remainingTime = q->inp_arr[j]->remainingTime;
                    q->inp_arr[j]->remainingTime = temp;

                    proNoTemp = (q->inp_arr[i]->processNumber);
                    (q->inp_arr[i]->processNumber) = (q->inp_arr[j]->processNumber);
                    (q->inp_arr[j]->processNumber) = proNoTemp;

                }
            }
    
        printf("(P%d, %d), ", (q->inp_arr[i]->processNumber), q->inp_arr[i]->remainingTime);
        timeQuantum = q->inp_arr[i]->remainingTime;
        remainingTimeCount += q->inp_arr[i]->remainingTime;
        }

        printf("]");
        printf("\n");
        int qitemCount = q->Rear - q->Front + 1;

        printf("No. of processes in queue: %d\n", qitemCount);


        // if (qitemCount > 0) {
        //     timeQuantum = (int) (remainingTimeCount / qitemCount);
        //     printf("Current Time Quantum: %d\n", timeQuantum);
        // }
        // else
        //     timeQuantum = remainingTimeCount;
    }
} 

int isEmpty(Queue* q) {
    return q->size == 0;
}

Process* peek(Queue* q){
    if (isEmpty(q)) {
        return NULL;
    }

    Process* tmp = q->inp_arr[q->Front];
    return tmp;
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

        printf("Process %d: ArrivalTime = %d, BurstTime = %d\n", p, arrivalTime, burstTime);
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

                printf("Added process %d to Queue!\n", (p->processNumber));
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

        // If queue is not empty, dequeue and put into CPU.
        if (!isEmpty(queue) && switchFlag) {
            // If switch is triggered (quantum exceeded), or start of algorithm

            // Add new process to CPU from front of Queue
            Process* tmp = dequeue(queue);
            cpu = tmp;


            // Else, the current process in CPU remains
            printf("Process %d added to CPU\n", cpu->processNumber);   
        }

        printf("Next Process: "); printProcess(cpu); 

        printf("count = %d, currTime = %d, timeElapsed = %d\n---\n", count, currTime, currTime+1);
        
        currTime++;             // Update time.
        switchFlag = false;     // Reset switch flag.

    }

    printf("\n\t PROCESS\t ARRIVAL TIME\t BURST TIME\t FINISH TIME\t TURNAROUND TIME\t WAITING TIME\t RESPONSE TIME\n");
    for (i = 0; i < processCount; i++) {
        processTurnaroundTime[i] = processFinishTime[i] - processArrivalTime[i];
        processWaitingTime[i] = processTurnaroundTime[i] - processBackupBurstTime[i];
        //processResponseTime[i] =   processArrivalTime[i];
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


    /*
        Output algorithm statistics
    */
    printf("---\nPrinting algorithm statistics...\n\n");
    printf("average turnaround time: %.2f\n", averageTurnaroundTime);
    printf("maximum turnaround time: %.2f\n", maxTurnaroundTime);
    printf("average waiting time: %.2f\n", averageWaitingTime);
    printf("maximum waiting time: %.2f\n", maxWaitingTime);
    //printf("time quantum: %d\n", timeQuantum);
    
}

int main ()
{
    defaultRoundRobin();
    return 0;
} 