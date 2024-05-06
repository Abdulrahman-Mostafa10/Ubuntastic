/*
 * This file is done for you.
 * Probably you will not need to change anything.
 * This file represents an emulated clock for simulation purpose only.
 * It is not a real part of operating system!
 */

#include "UI.h"
char* lineToPrint;
int shmid;

/* Clear the resources before exit */
void cleanup(int signum)
{
    shmctl(shmid, IPC_RMID, NULL);
    printLine("\nClock terminating!\n",RED);
    exit(0);
}

/* This file represents the system clock for ease of calculations */
int main(int argc, char *argv[])
{
    lineToPrint=(char*)malloc(LINE_SIZE*sizeof(char));
    printLine("Clock starting\n",GRN);
    signal(SIGINT, cleanup);
    clk_t clk = 0;
    // Create shared memory for one integer variable 4 bytes
    shmid = shmget(SHKEY, 4, IPC_CREAT | 0644);
    if ((long)shmid == -1)
    {
        printError("Error in creating shm!");
        exit(-1);
    }
    int *shmaddr = (int *)shmat(shmid, (void *)0, 0);
    if ((long)shmaddr == -1)
    {
        printError("Error in attaching the shm in clock!");
        exit(-1);
    }
    *shmaddr = clk; /* initialize shared memory */
    while (1)
    {
        sleep(1);
        sprintf(lineToPrint,"\x1B\033[37m\033[41mClock = %d",++(*shmaddr));
        printLine(lineToPrint,GRN);
    }
}