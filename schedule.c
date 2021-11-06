#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

void fcfs_waiting_time(int brstNoArr[], int arrLen, int brstLenArr[],int wait[],int turnAround[],int arrvlTime[]){
  int timeSpan[arrLen];
  timeSpan[0] = arrvlTime[0];
  wait[0] = 0;

  for(int i = 1; i < arrLen; i++){
    timeSpan[i] = timeSpan[i-1] + brstLenArr[i-1];

    wait[i] = timeSpan[i] - arrvlTime[i];

    if(wait[i] < 0){
      wait[i] = 0;
    }
  }
}
void fcfs(int brstNoArr[],int arrLen, int brstLenArr[], int arrvlTime[]){
  int turnAround[arrLen];
  int wait[arrLen];

	fcfs_waiting_time(brstNoArr,arrLen,brstLenArr,wait,turnAround,arrvlTime);
	for(int i = 0; i < arrLen; i++){
    turnAround[i] = brstLenArr[i] + wait[i];     
    }
  int totalTurnAround = 0;
  for(int i = 0; i < arrLen; i++){
    totalTurnAround += turnAround[i];
  }

  float fcfsAvgTurnAround = (float) totalTurnAround / (float) arrLen;

  printf("Avg TurnaroundTime: %.3f \n", fcfsAvgTurnAround);
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
	
	// FCFS
  fcfs(brstNoArr, arrLen, brstLenArr, arrvlTime);
}