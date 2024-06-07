#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

#define SORT_BY_ARRIVAL 0
#define SORT_BY_BURST 1
#define MAX 1000

typedef struct {
    int PID;
    int Arrival, Burst;
    int Start, Finish, Waiting, Response,  TaT;
    int OA, OB;
} PCB;

void setPCBDefault(PCB *Q)
{
    Q->PID = MAX;
    Q->Arrival = MAX;
    Q->Burst = MAX;
    Q->Start = 0;
    Q->Finish = 0;
    Q->Waiting = 0;
    Q->Response = 0;
    Q->TaT = 0;
    Q->OA = 0;
    Q->OB = 0;
}

void inputProcesses(int n, PCB P[])
{
    for (int i = 0; i < n; i++)
    {
        printf("Enter arrival time and burst time for process %d: ", i + 1);
        scanf("%d%d", &P[i].Arrival, &P[i].Burst);
        P[i].PID = i + 1;
    }
}

int min(int a, int b)
{
    if (a < b)
        return a;
    return b;
}

void swap(PCB *a, PCB *b)
{
    PCB temp = *a;
    *a = *b;
    *b = temp;
}

int partition(PCB P[], int low, int high, int Criteria) 
{
    PCB pivot = P[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) 
    {
        if ((Criteria == SORT_BY_ARRIVAL && P[j].Arrival <= pivot.Arrival)
        || (Criteria == SORT_BY_BURST && P[j].Burst <= pivot.Burst)) 
        {
            i++;
            swap(&P[i], &P[j]);
        }
    }
    swap(&P[i + 1], &P[high]);
    return (i + 1);
}

void quickSort(PCB P[], int low, int high, int Criteria) 
{
    if (low < high) 
    {
        int pi = partition(P, low, high, Criteria);
        quickSort(P, low, pi - 1, Criteria);
        quickSort(P, pi + 1, high, Criteria);
    }
}

void pushProcess(int *n, PCB P[], PCB Q) 
{
    P[*n] = Q;
    (*n)++;
}

void removeProcess(int *n, int id, PCB P[]) 
{
    for (int i = id; i < *n; i++) 
    {
        P[i] = P[i + 1];
    }
    (*n)--;
}

void printProcesses(int n, PCB P[]) {
    printf("| %-4s | %-8s | %-8s | %-12s | %-12s | %-12s | %-12s | %-12s |\n",
           "PID", "Arrival", "Burst", "Start Time", "Finish Time", "Waiting Time", "Response Time", "Turnaround Time");
    printf("|------|----------|----------|--------------|--------------|--------------|---------------|-----------------|\n");

    for (int i = 0; i < n; i++) {
        printf("| %-4d | %-8d | %-8d | %-12d | %-12d | %-12d | %-12d | %-12d |\n",
               P[i].PID, P[i].Arrival, P[i].Burst, P[i].Start, P[i].Finish, P[i].Waiting, P[i].Response, P[i].TaT);
    }

    printf("\n");
}

void printRes(PCB P[], int n)
{
    float avgWaiting = 0;
    float avgResponse = 0;
    float avgTurnaround = 0;

    for (int i = 0; i < n; i++)
    {
        avgResponse += P[i].Response;
        avgWaiting += P[i].Waiting;
        avgTurnaround += P[i].TaT;

    }

    avgResponse /= n;
    avgTurnaround /= n;
    avgWaiting /= n;   

    printf("\nAverage Waiting Time: %.2f", avgWaiting);
    printf("\nAverage Response Time: %.2f", avgResponse);
    printf("\nAverage Turn Around Time: %.2f\n", avgTurnaround);
}

int main()
{
    PCB Input[20];
    PCB ReadyQueue[20];
    PCB TerminatedArray[20];

    for (int i = 0; i < 20; i++)
    {
        setPCBDefault(&Input[i]);
        setPCBDefault(&ReadyQueue[i]);
        setPCBDefault(&TerminatedArray[i]);
    }

    int NumberOfProcess;
    int timeNow = 0;
    int quantumTime;
    
    printf("Please input number of Process: ");
    scanf("%d", &NumberOfProcess);

    printf("Please input quantum time: ");
    scanf("%d", &quantumTime);

    inputProcesses(NumberOfProcess, Input);

    int remain = NumberOfProcess;
    int ready = 0;
    int terminated = 0; 

    quickSort(Input, 0, NumberOfProcess - 1, SORT_BY_ARRIVAL);

    printProcesses(NumberOfProcess, Input);

    pushProcess(&ready, ReadyQueue, Input[0]);
    removeProcess(&remain, 0, Input);

    ReadyQueue[0].Start = ReadyQueue[0].Arrival;
    ReadyQueue[0].Response = 0;
    ReadyQueue[0].OA = ReadyQueue[0].Arrival;
    ReadyQueue[0].OB = ReadyQueue[0].Burst;

    while (terminated < NumberOfProcess)
    {
        while (remain > 0)
        {
            if (Input[0].Arrival <= timeNow + min(quantumTime, ReadyQueue[0].Burst))
            {
                Input[0].OA = Input[0].Arrival;
                Input[0].OB = Input[0].Burst;
                pushProcess(&ready, ReadyQueue, Input[0]);
                removeProcess(&remain, 0, Input);
                continue;
            }
            else break;
        }

        if (ready > 0)
        {
            if (ReadyQueue[0].Burst <= quantumTime)
            {
                timeNow = ReadyQueue[0].Start + ReadyQueue[0].Burst;
                ReadyQueue[0].Finish = timeNow;
                pushProcess(&terminated, TerminatedArray, ReadyQueue[0]);
                removeProcess(&ready, 0, ReadyQueue);

                if (ready == 0 && remain > 0 && Input[0].Arrival > timeNow)
                {
                    if (terminated == NumberOfProcess)
                    {
                        break;
                    }
                    else
                    {
                        Input[0].OA = Input[0].Arrival;
                        Input[0].OB = Input[0]. Burst;
                        pushProcess(&ready, ReadyQueue, Input[0]);
                        removeProcess(&remain, 0, Input);
                        continue;
                    }
                }
                else 
                {
                    if (terminated == NumberOfProcess)
                    {
                        break;
                    }
                    else
                    {
                        ReadyQueue[0].Start = timeNow;
                        if (ReadyQueue[0].Response != 0)
                        {
                            continue;
                        }
                        else
                        {
                            ReadyQueue[0].Response = ReadyQueue[0].Start - ReadyQueue[0]. Arrival;
                            continue;
                        }
                    }
                }
            }
            else
            {
                timeNow = ReadyQueue[0].Start + quantumTime;
                ReadyQueue[0].Finish = timeNow;
                ReadyQueue[0].Burst -= quantumTime;
                ReadyQueue[0].Arrival = timeNow;
                pushProcess(&ready, ReadyQueue, ReadyQueue[0]);
                removeProcess(&ready, 0, ReadyQueue);

                ReadyQueue[0].Start = timeNow;

                if (ReadyQueue[0].Response != 0)
                {
                    continue;
                }
                else
                {
                    ReadyQueue[0].Response = ReadyQueue[0].Start - ReadyQueue[0]. Arrival;
                    continue;
                }
            }
        }
    }

    printf("\n===== Round Robin Scheduling =====\n");

    for (int i = 0; i < terminated; i++)
    {
        TerminatedArray[i].TaT = TerminatedArray[i].Finish - TerminatedArray[i].OA;
        TerminatedArray[i].Waiting = TerminatedArray[i].Finish - TerminatedArray[i].OA - TerminatedArray[i].OB;
    }

    printProcesses(terminated, TerminatedArray);
    printRes(TerminatedArray, terminated);

    return 0;
}