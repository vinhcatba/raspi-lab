#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

pthread_t tid[2];
int counter;
pthread_mutex_t lock;

void* writeToFile(void* arg)
{
    // khoa mutex
    pthread_mutex_lock(&lock);

    FILE *fd = (FILE *)arg;
    unsigned long i = 0;
    counter += 1;
    printf("Thread %d is writing\n", counter);
    fprintf(fd, "Thread %d write: %d. ", counter, rand()%10);
    // doi 1 khoang thoi gianS
    for (i = 0; i < (0xFFFFFFFF); i++)
        ;
    fprintf(fd, "Thread %d finished\n", counter);
    printf("Thread %d finished\n", counter);
    
    // tra mutex
    pthread_mutex_unlock(&lock);


	return NULL;
}

int main(void)
{
    // mo file duoi dang append (ghi vao cuoi file)
    FILE *fd;
    fd = fopen("data.txt", "a");
    if(fd == NULL){
        exit(-1);
    }
    // khoi tao mutex
	if (pthread_mutex_init(&lock, NULL) != 0) {
		printf("\n mutex init has failed\n");
		return 1;
	}
    // tao 2 thread
    for (int i=0; i<2; i++){
        pthread_create(&(tid[i]), NULL, writeToFile, (void *)fd);
    }

	for (int i=0; i<2; i++){
        pthread_join(tid[i], NULL);
    }
    // huy bo mutex va dong file
	pthread_mutex_destroy(&lock);
    close(fd);
	return 0;
}
