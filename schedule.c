#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

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

  printf("Avg TurnaroundTime: %.3f \n", avgTurnAround);
}

int main(int argc, char** argv){
  int arrLen = 0;
	// first file read for arrLength
  FILE *fNewFile = fopen(argv[1], "r");
  if (fNewFile == NULL) {
		perror("File is NULL!\n");
		exit(1);
	}
  char lenLine[256];
  while (fgets(lenLine, 256, fNewFile)) {
		arrLen++;
	}
	fclose(fNewFile);

	// arr inits
	int brstNoArr[arrLen];
	int arrvlTime[arrLen];
	int brstLenArr[arrLen];

	// second file read for arr items
	FILE *sNewFile = fopen(argv[1], "r");
  if (sNewFile == NULL) {
		perror("File is NULL!\n");
		exit(1);
	}
	char arrLine[256];
	int itemIndex = 0;
	while (fgets(arrLine, 256, sNewFile)) {
		char *newStr;
		// brstNo
		newStr = strtok(arrLine, " ");
		brstNoArr[itemIndex] = atoi(newStr);

		// arrvlTime
		newStr = strtok(NULL, " ");
		arrvlTime[itemIndex] = atoi(newStr);

		// brstLen
		newStr = strtok(NULL, " ");
		brstLenArr[itemIndex] = atoi(newStr);

		itemIndex++;
	}
  fclose(sNewFile);

	// control
	printf("File Control, Herkes fileini atsin!\n");
	for (int i = 0; i < arrLen; i++){
		printf("Arr index: %d => Burst no: %d - Arrival T: %d - Burst Length: %d\n", 
			i, brstNoArr[i], arrvlTime[i], brstLenArr[i]);		
	}
	printf("\n\n\n");

	// example code
	int processes[] = {1,2,3};
  int n = sizeof(processes) / sizeof(processes[0]);

  int burstTime[] = {24,3,3};

  int arrivaltime[] = {0,3,5};

  fcfs(processes,n,burstTime,arrivaltime);
}