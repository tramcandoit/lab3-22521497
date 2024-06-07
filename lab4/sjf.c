#include <stdio.h>
#include <stdlib.h>
#define SORT_BY_ARRIVAL 0
#define SORT_BY_PID 1
#define SORT_BY_BURST 2
#define SORT_BY_START 3

typedef struct
{
    int iPID;
    int iArrival, iBurst;
    int iStart, iFinish, iWaiting, iResponse, iTaT;
} PCB;

int generateRandomNumber (int minNumber, int maxNumber) {
    return rand() % (maxNumber - minNumber + 1) + minNumber;
}

void inputProcess(int n, PCB P[])
{
    for (int i = 0; i < n; i++)
    {
        printf("Enter arrival time and burst time for process %d: ", i + 1);
        scanf("%d%d", &P[i].iArrival, &P[i].iBurst);
        P[i].iPID = i + 1;
    }
}

void printProcess(int n, PCB P[])
{
    printf("%-4s | %-8s | %-8s | %-12s | %-12s | %-12s | %-12s |   %-12s |\n", 
        "PID", "Arrival", "Burst", "Start Time", "Finish Time", "Waiting Time", "Response Time", "Turnaround Time");
    printf("|------|-----------|-----------------|----------------|---------------|-----------------|-----------------|\n");
    for (int i = 0; i < n; i++) {
        printf("%-4d | %-8d | %-8d | %-12d | %-12d | %-12d | %-12d |   %-12d |\n",
        P[i].iPID, P[i].iArrival, P[i].iBurst, P[i].iStart, P[i].iFinish, P[i].iWaiting, P[i].iResponse, P[i].iTaT);
    }

    printf("\n");
}

void exportGanttChart(int n, PCB P[])
{
    printf("Gantt Chart:\n");
    for (int i = 0; i < n; i++)
    {
        printf("| Process %d ", P[i].iPID);
    }
    printf("|\n");
}

void pushProcess(int *n, PCB P[], PCB Q)
{
    P[*n] = Q;
    (*n)++;
}

void removeProcess(int *n, int index, PCB P[])
{
    for (int i = index; i < (*n) - 1; i++)
    {
        P[i] = P[i + 1];
    }
    (*n)--;
}

int swapProcess(PCB *P, PCB *Q)
{
    PCB temp = *P;
    *P = *Q;
    *Q = temp;
    return 0;
}

int partition(PCB P[], int low, int high, int iCriteria)
{
    PCB pivot = P[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++)
    {
        if ((iCriteria == SORT_BY_ARRIVAL && P[j].iArrival <= pivot.iArrival)
        || (iCriteria == SORT_BY_BURST && P[j].iBurst <= pivot.iBurst))
        {
            i++;
            swapProcess(&P[i], &P[j]);
        }
    }
    swapProcess(&P[i + 1], &P[high]);
    return (i + 1);
}

void quickSort(PCB P[], int low, int high, int iCriteria)
{
    if (low < high)
    {
        int pi = partition(P, low, high, iCriteria);

        quickSort(P, low, pi - 1, iCriteria);
        quickSort(P, pi + 1, high, iCriteria);
    }
}

void printRes(PCB P[], int n) 
{
    float avgWaiting = 0;
    float avgResponse = 0;
    float avgTurnARound = 0;

    for (int i = 0; i < n; i++) {
        avgResponse += P[i].iResponse;
        avgWaiting += P[i].iWaiting;
        avgTurnARound += P[i].iTaT;
    }

    avgResponse /= n;
    avgWaiting /= n;
    avgTurnARound /= n;

    printf("\nAverage Waiting Time: %.2f", avgWaiting);
    printf("\nAverage Response Time: %.2f", avgResponse);
    printf("\nAverage Turn Around Time: %.2f\n", avgTurnARound);
}

int main()
{
    PCB Input[10];
    PCB ReadyQueue[10];
    PCB TerminatedArray[10];
    
    int numberOfProcess;
    int timeNow = 0;

    printf("Please input number of Process: "); 
    scanf("%d", &numberOfProcess);
    inputProcess(numberOfProcess, Input);

    int remain = numberOfProcess;
    int ready = 0;
    int terminated = 0;

    quickSort(Input, 0, numberOfProcess - 1, SORT_BY_ARRIVAL);

    pushProcess(&ready, ReadyQueue, Input[0]);
    removeProcess(&remain, 0, Input);
    
    ReadyQueue[0].iStart = ReadyQueue[0].iArrival;
    ReadyQueue[0].iFinish = ReadyQueue[0].iStart + ReadyQueue[0].iBurst;
    ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival; 
    ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse;
    ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival;

    while (terminated < numberOfProcess) 
    {
        while (remain > 0) 
    {
            if(Input[0].iArrival <= ReadyQueue[0].iFinish) 
            {
                pushProcess(&ready, ReadyQueue, Input[0]);
                removeProcess(&remain, 0, Input);
                continue;
            }
            else break;
        }
        if (ready > 0) 
        {
            timeNow = ReadyQueue[0].iFinish;
            pushProcess(&terminated, TerminatedArray, ReadyQueue[0]);
            removeProcess(&ready, 0, ReadyQueue);

            if (ready == 0 && remain > 0 && Input[0].iArrival > timeNow) 
            {
                if (terminated == numberOfProcess) break;
                else 
                {
                    pushProcess(&ready, ReadyQueue, Input[0]);
                    removeProcess(&remain, 0, Input);

                    ReadyQueue[0].iStart = ReadyQueue[0].iArrival;
                    ReadyQueue[0].iFinish = ReadyQueue[0].iStart + ReadyQueue[0].iBurst;
                    ReadyQueue[0].iResponse =ReadyQueue[0].iStart - ReadyQueue[0].iArrival;
                    ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse;
                    ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival;
                }
            }
            else 
            {
                quickSort(ReadyQueue, 0, ready, SORT_BY_BURST);

                ReadyQueue[0].iStart = TerminatedArray[terminated - 1].iFinish;
                ReadyQueue[0].iFinish = ReadyQueue[0].iStart + ReadyQueue[0].iBurst;
                ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival;
                ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse;
                ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival;
            }
        }
    }
    printf("\n ========= SJF SCHEDULING ========== \n");
    printProcess(terminated, TerminatedArray);
    printRes(TerminatedArray, numberOfProcess);
    return 0;
}
