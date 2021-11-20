#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

typedef struct node{
	int data; //arrvTime
	int priority; //burstTime
	
	struct node* next;
} Node;

Node* front = NULL, *rear = NULL;

void enqueue(int arrv, int burst){
	Node* newNode = malloc(sizeof(Node));
	newNode->data = arrv;
	newNode->priority = burst;
	newNode->next = NULL;
	
	if(front == NULL || rear == NULL){
		front = rear = newNode;
	}
	else{
		rear->next = newNode;
		rear = newNode;
	}
}

Node* dequeue(){
	Node* temp = NULL;
	if(front != NULL){
		temp = front;
		front = front->next;
		
		if(front == NULL){
			rear = NULL;
		}
	}
	return temp;
}

Node* newNode(int d, int p){
	Node* temp = (Node*) malloc(sizeof(Node));
	temp -> data = d;
	temp -> priority = p;
	temp -> next = NULL;

	return temp;
}

Node* pop(Node** head){
	Node *temp = *head;
	(*head) = (*head)->next;
	return temp;
} 

void push(Node** head, int d, int p){
	Node* start = (*head);

	Node *temp = newNode(d,p);

	if((*head) -> priority >p){
		temp -> next = *head;
		(*head) = temp;
	}

	else{
		while(start->next != NULL && start->next->priority < p){
			start = start->next;
		}
		temp->next = start->next;
		start->next	= temp;
	}
}


void rrHelper(int brstNoArr[], int arrvlTime[], int brstLenArr[], int arrLen, int quantum, bool check[], int currentTime, bool flag){
	for(int i = 0; i < arrLen; i++){
		if(currentTime >= arrvlTime[i] && !check[i]){
			if(flag){
			enqueue(arrvlTime[i],brstLenArr[i]-1);
			check[i] = true;
			}
			else{
			enqueue(arrvlTime[i],brstLenArr[i]);
			check[i] = true;
			}
		}
	}
}

void rr(int brstNoArr[], int arrvlTime[], int brstLenArr[], int arrLen, int quantum, bool check[]){
	int doneCount = 0;
	int currentTime = 0;
	int totalTA = 0;
	while(doneCount != arrLen){
		if(front == NULL){
			rrHelper(brstNoArr,arrvlTime,brstLenArr,arrLen,quantum,check,currentTime, true);
			currentTime++;
		}
		else{
			Node* temp = dequeue();
			int timeElapsed = temp->priority;
			if(timeElapsed > quantum){
				timeElapsed = quantum;
			}
			for(int j = 1; j <= timeElapsed; j++){
				rrHelper(brstNoArr,arrvlTime,brstLenArr,arrLen,quantum,check,currentTime,false);
				currentTime = currentTime + 1;
			}	
			if(temp->priority > quantum){
				enqueue(temp->data,temp->priority-quantum);
			}
			else{
				totalTA = totalTA + currentTime - temp->data; 
				free(temp);
				doneCount++;
			}
		}
	}
	float rrAvgTurnAround = (float) totalTA / (float) arrLen;
  printf("RR: %f \n", rrAvgTurnAround);
}

void srtf(int brstNoArr[], int arrvlTime[], int brstLenArr[], int arrLen, bool check[]){
	int doneCount = 0;
	int currentTime = 0;
	int totalTA = 0;
	Node *head = NULL;
	while(doneCount != arrLen){
		for(int i = 0; i < arrLen; i++){
			if(currentTime >= arrvlTime[i] && check[i]){
				if(head == NULL){
					head = newNode(arrvlTime[i],brstLenArr[i]);
					check[i] = false;
				}
				else{
					push(&head,arrvlTime[i],brstLenArr[i]);
					check[i] = false;
				}	
			}
		}

		if(head == NULL){
			currentTime++;
		}
		else{
			Node* temp = pop(&head);
			if(temp->priority != 1){
				if(head == NULL){
					head = newNode(temp->data,temp->priority-1);
				}
				else{
					push(&head,temp->data,temp->priority-1);
				}
			currentTime++;
			free(temp);
			}
			else{
				currentTime++;
				totalTA = totalTA + (currentTime - temp->data);
				free(temp);
				doneCount++;
			}
		}
	}
	float srtfAvgTurnAround = (float) totalTA / (float) arrLen;
  printf("SRTF: %f \n", srtfAvgTurnAround);
}

void sjf(int brstNoArr[], int arrvlTime[], int brstLenArr[], int arrLen, bool check[]){
	int doneCount = 0;
	int currentTime = 0;
	int totalTA = 0;
	Node *head = NULL;
	while(doneCount != arrLen){
		for(int i = 0; i < arrLen; i++){
			if(currentTime >= arrvlTime[i] && !check[i]){
				if(head == NULL){
					head = newNode(arrvlTime[i],brstLenArr[i]);
					check[i] = true;
				}
				else{
					push(&head,arrvlTime[i],brstLenArr[i]);
					check[i] = true;
				}	
			}
		}
		if(head == NULL){
			currentTime++;
		}	
		else{
			Node* temp = pop(&head);
			currentTime = currentTime + temp->priority;
			totalTA = totalTA + (currentTime - temp->data);
			free(temp);
			doneCount++;
		}
	}
	float sjfAvgTurnAround = (float) totalTA / (float) arrLen;
  printf("SJF: %f \n", sjfAvgTurnAround);
}

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

void fcfs(int brstNoArr[], int arrvlTime[], int brstLenArr[], int arrLen){
  int turnAround[arrLen];
  int wait[arrLen];

	fcfs_waiting_time(brstNoArr,arrLen,brstLenArr,wait,turnAround,arrvlTime);
	// turnaround time
	for(int i = 0; i < arrLen; i++){
    turnAround[i] = brstLenArr[i] + wait[i];     
  }
  int totalTurnAround = 0;
  for(int i = 0; i < arrLen; i++){
    totalTurnAround += turnAround[i];
  }

  float fcfsAvgTurnAround = (float) totalTurnAround / (float) arrLen;
  printf("FCFS: %f \n", fcfsAvgTurnAround);
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
	bool checkArr[arrLen];

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

		checkArr[itemIndex] = false;

		itemIndex++;
	}
  fclose(sNewFile);

	int quantum = atoi(argv[2]);

	// control
	printf("File Control, Herkes fileini atsin!\n");
	for (int i = 0; i < arrLen; i++){
		printf("Arr index: %d => Burst no: %d - Arrival T: %d - Burst Length: %d\n", 
			i, brstNoArr[i], arrvlTime[i], brstLenArr[i]);		
	}
	printf("%d\n\n\n", quantum);
	
	// Algorithm Calls
  fcfs(brstNoArr, arrvlTime, brstLenArr, arrLen);
  sjf(brstNoArr, arrvlTime, brstLenArr, arrLen, checkArr);
  srtf(brstNoArr, arrvlTime, brstLenArr, arrLen, checkArr);
  rr(brstNoArr, arrvlTime, brstLenArr, arrLen, quantum, checkArr);
}