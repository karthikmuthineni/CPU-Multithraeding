#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sched.h>
#include <unistd.h>
#include <sys/wait.h>
#define M 1000
#define K 30
#define N 1000
#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); \
} while (0)
int A [M][K] = { {1,4}, {2,5}, {3,6} };
int B [K][N] = { {8,7,6}, {5,4,3} };
int C [M][N];
struct v {
int i; /* row */
int j; /* column */
};
void *runner(void *param); /* the thread */
int main(int argc, char *argv[]) {
int i,j,v,s, count = 0;
int num_threads=6;
pthread_t p[num_threads];
cpu_set_t my_set; /* Define your cpu_set bit mask. */
CPU_ZERO(&my_set); /* Initialize it all to 0 */
CPU_SET(3 , &my_set);
sched_setaffinity(0, sizeof(cpu_set_t), &my_set);
cpu_set_t set;
int parentCPU, childCPU;
int nloops, k;
if (argc != 4) {
fprintf(stderr, "Usage: %s parent-cpu child-cpu num-loops\n",
argv[0]);
exit(EXIT_FAILURE);
}
parentCPU = atoi(argv[1]);
childCPU = atoi(argv[2]);
nloops = atoi(argv[3]);
CPU_ZERO(&set);
switch (fork()) {
case -1: /* Error */
errExit("fork");
case 0: /* Child */
CPU_SET(childCPU, &set);
if (sched_setaffinity(getpid(), sizeof(set), &set) == -1)
errExit("sched_setaffinity");
for (k = 0; k < nloops; k++)
getppid();
exit(EXIT_SUCCESS);
default: /* Parent */
CPU_SET(parentCPU, &set);
if (sched_setaffinity(getpid(), sizeof(set), &set) == -1)
errExit("sched_setaffinity");
for (k = 0; k < nloops; k++)
getppid();
wait(NULL); /* Wait for child to terminate */
}
wait;
for(i = 0; i < M; i++) {
for(j = 0; j < N; j++) {
//Assign a row and column for each thread
struct v *data = (struct v *) malloc(sizeof(struct v));
data->i = i;
data->j = j;
/* Now create the thread passing it data as a parameter */
pthread_t tid; //Thread ID
pthread_attr_t attr; //Set of thread attributes
//Get the default attributes
pthread_attr_init(&attr);
//Create the thread
while (i<4)
{
pthread_create(&tid,&attr,runner,data);
i++;
}
//Make sure the parent waits for all thread to complete
pthread_join(tid, NULL);
count++;
}
}
//Print out the resulting matrix
for(i = 0; i < M; i++) {
for(j = 0; j < N; j++) {
printf("%d ", C[i][j]);
}
printf("\n");
}
}
//The thread will begin control in this function
void *runner(void *param) {
struct v *data = param; // the structure that holds our data
int n, sum = 0; //the counter and sum
//Row multiplied by column
for(n = 0; n< K; n++){
sum += A[data->i][n] * B[n][data->j];
}
//assign the sum to its coordinate
C[data->i][data->j] = sum;
//Exit the thread;
pthread_exit(0); }
