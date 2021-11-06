#include <stdio.h>
void fcfs_waiting_time(int processes[], int n, int burstTime[],int wait[],int turnAround[],int arrivalTime[]){
    int timeSpan[n];
    timeSpan[0] = arrivalTime[0];
    wait[0] = 0;

    for(int i = 1; i < n; i++){
        timeSpan[i] = timeSpan[i-1] + burstTime[i-1];

        wait[i] = timeSpan[i] - arrivalTime[i];

        if(wait[i] < 0){
            wait[i] = 0;
        }
    }
}
void fcfs_turn_around(int processes[],int n, int burstTime[],int wait[], int turnAround[]){
    for(int i = 0; i < n; i++){
        turnAround[i] = burstTime[i] + wait[i];     
    }
}

void fcfs(int processes[],int n, int burstTime[], int arrivalTime[]){
    int turnAround[n];
    int wait[n];

    fcfs_waiting_time(processes,n,burstTime,wait,turnAround,arrivalTime);
    fcfs_turn_around(processes,n,burstTime,wait,turnAround);

    int totalTurnAround = 0;
    for(int i = 0; i < n; i++){
        totalTurnAround += turnAround[i];
    }

    float avgTurnAround = (float) totalTurnAround / (float) n;

    printf("%.3f", avgTurnAround);
}

int main(){
    int processes[] = {1,2,3};
    int n = sizeof(processes) / sizeof(processes[0]);

    int burstTime[] = {24,3,3};

    int arrivaltime[] = {0,3,5};

    fcfs(processes,n,burstTime,arrivaltime);
}