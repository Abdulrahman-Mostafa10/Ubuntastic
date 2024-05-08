#include "UI.h"
queue_t *readFile(char *filePath, queue_t *Pqueue)
{
    int numProcesses = 0;
    // Initialize the Process Queue

    int id, runTime, arrivalTime, priority, memSize;
    char firstLine[100];
    FILE *inputFile = fopen(filePath, "r");
    if (!inputFile)
    {
        printError("Error in opening the processes.txt file\n");
        return NULL;
    }
    fgets(firstLine, sizeof(firstLine), inputFile); // Ignoring the first line

    while (fscanf(inputFile, "%d\t%d\t%d\t%d\t%d\n", &id, &arrivalTime, &runTime, &priority, &memSize) == 5)
    {
        // Read the process data and creating a process

        int info[] = {id, arrivalTime, runTime, priority, memSize};
        process_t *pd = createProcess(info);
        enqueue(Pqueue, pd);
        numProcesses++;
    }
    fclose(inputFile);
// Testing
#ifdef DEBUG
    printLine("The processes are read from the file\n", GRN);
    printQueue(Pqueue, MAG);
#endif
    printf("Number of processes: %d\n", numProcesses);
    int info[] = {0, 0, 0, 0, 0};
    process_t *dummy = createProcess(info);
    enqueue(Pqueue, dummy);
    return Pqueue;
}

void getUserInput(int *schedAlgo, int *timeSlice) // Will get updated and support GUI
{
Get_Scheduling_Algorithm:
    system("clear"); // Clear the terminal
    printLine("|| Choose a Scheduling Algorithm ||\n", BLU);
    printLine("For SRTN, Enter 0\nFor HPF, Enter 1\nFor RR, Enter 2\n ", GRN);
    printLine("Algorithm: ", WHT);
    scanf("%d", schedAlgo);
    if (*schedAlgo > 2)
    {
        printLine("Invalid Option ->\n", RED);
        sleepMilliseconds(500);
        goto Get_Scheduling_Algorithm;
    }

    if (*schedAlgo == 2)
    {
    // For RR get the time slice
    Get_RR_timeSlice:
        printLine("||Enter the Time Slice||\nTime: ", BLU);
        scanf("%d", timeSlice);
        if (*timeSlice < 0)
        {
            printLine("\nInvalid Option -> ", RED);
            goto Get_RR_timeSlice;
        }
    }
    system("clear");
}
void displayScheduler(int algo)
{
    printLine("The Scheduler Algorithm is: ", BLU);
    system("clear");
    char *Line = "--------------------SCHEDULER ALGORITHM--------------------";
    switch (algo)
    {
    case RR_t:
        sprintf(Line, "--------------------RR ALGORITHM--------------------\n");
        printLine(Line, BLU);
        break;
    case HPF_t:
        sprintf(Line, "--------------------HPF ALGORITHM--------------------\n");
        printLine(Line, BLU);
        break;
    case SRTN_t:
        sprintf(Line, "--------------------SRTN ALGORITHM--------------------\n");
        printLine(Line, BLU);
        break;
    default:
        printLine("INVALID SCHEDULING ALGORITHM\n", RED);
        break;
    }
}

void printLine(char line[], char *clr)
{
    printf("%s%s%s\n", clr, line, NRM);
}

void print(char line[], char *clr)
{
    printf("%s%s%s", clr, line, NRM);
}

void printError(char errorMsg[])
{
    perror(errorMsg);
}

void insertIntoLog(state_t state, float *pInfo)
{
    FILE *file = fopen("./scheduler.log", "a");
    if (!file)
    {
        printf("Error in opening the file.. ");
        exit(-1);
    }

    char st[30];
    switch (state)
    {
    case STARTED:
        strcpy(st, "⏳ allocated ⏳");
        break;
    case STOPPED:
        strcpy(st, "⛔️ freed ⛔️");
        break;
    case FINISHED:
        strcpy(st, "✅ freed ✅");
        break;
    case RESUMED:
        strcpy(st, "▶️ allocated ▶️");
        break;
    }

    fprintf(file, "AT time %d %s %d bytes for process %d from %d to %d ", getClk(), st, (int)pInfo[1], (int)pInfo[0], (int)pInfo[2], (int)pInfo[3]);

    fprintf(file, "\n");
    fclose(file);
}

void generatePrefFile(float *statistics)
{
    FILE *file = fopen("./scheduler.perf", "w");
    if (!file)
    {
        printf("can't open the file\n");
        exit(-1);
    }

    printLine("The Scheduler Statistics:\n", BLU);
    printLine("CPU utilization = ", GRN);
    printf("%.2f %%\n", statistics[0]);
    printLine("Avg WTA = ", GRN);
    printf("%.2f\n", statistics[1]);
    printLine("Avg Waiting = ", GRN);
    printf("%.2f\n", statistics[2]);
    printLine("Std WTA = ", GRN);
    printf("%.2f\n", statistics[3]);
    fprintf(file, "CPU utilization = %.2f %%\n", statistics[0]);
    fprintf(file, "Avg WTA = %.2f\n", statistics[1]);
    fprintf(file, "Avg Waiting = %.2f\n", statistics[2]);
    fprintf(file, "Std WTA = %.2f\n", statistics[3]);
    fclose(file);
}

void printProcess(void *data, char *clr)
{
    process_t *temp = (process_t *)data;
    if (temp != NULL)
        printf("ID: %d, AT: %d, RT: %d, Priority: %d, RemT: %d State:%s", temp->ID, temp->AT, temp->RT, temp->priority, temp->RemT, RED);
    switch (temp->state)
    {
    case FINISHED:
        printf("FINISHED");
        break;

    case WAITING:
        printf("WAITING");
        break;

    default:
        printf("STARTED");
        break;
    }
    printf("%s\n", NRM);
}

void printQueue(queue_t *queue, char *clr)
{
    if (isEmptyQueue(queue))
    {
        printLine("The Queue is Empty\n", RED);
        return;
    }
    printf("the queue size is %d and has:\n", queue->size);
    if (queue->front != NULL)
    {
        node_t *temp = queue->front;
        while (temp != NULL && temp->data != NULL)
        {
            if (temp->data != NULL)

                printProcess(temp->data, clr);

            temp = temp->next;
        }
    }
}

void printHeap(minHeap_t *heap, char *clr)
{
    if (isEmptyHeap(heap))
    {
        printLine("The Heap is Empty\n", RED);
        return;
    }

    printf("The heap size is %d and has:\n", heap->size);
    for (int i = 0; i < heap->size; i++)
    {
        printf("here is am");
        printProcess((void *)heap->arr[i], clr);
    }
}