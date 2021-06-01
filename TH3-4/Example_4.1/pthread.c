#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

#define NUM		5

pthread_mutex_t mutex_file[NUM];

static char stt[][16] = {"first", "second", "third", "fourth", "fifth"};

void *write_file(void *arg) {
	int *fd = (int *)arg;
	char data[64];
	int random, id;

	for (int i = 0; i < 10e6; ++i) {
		srand(clock());
		random = rand();
		id = random % NUM;
		random = random % 10000;
		sprintf(data, "%d ", random);

		pthread_mutex_lock(&mutex_file[id]);
		if (write((fd[id]), &data, strlen(data)) < 0) {
			pthread_mutex_unlock(&mutex_file[id]);
			perror("Error from write");
			break;
		}
		pthread_mutex_unlock(&mutex_file[id]);
	}

	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	int ret;
	char path[64] = "";
	int fd[NUM];
	pthread_t ptid[NUM];

	for (int i = 0; i < NUM; ++i) {
		sprintf(path, "%s-data.txt", stt[i]);
		fd[i] = open(path, O_RDWR | O_CREAT, 0644);
		if (fd[i] < 0) {
			perror("Could not open file");
			return -1;
		}

		ret = pthread_mutex_init((mutex_file + i), NULL);
                if (ret < 0) {
                        perror("Error from pthread_mutex_init");
                        return -1;
                }

		ret = pthread_create(ptid + i, NULL, write_file, (void *)fd);
                if (ret < 0) {
                        perror("Error from pthread_create");
                        return -1;
                }
	}

	for (int i = 0; i < NUM; ++i) 
		pthread_join(ptid[i], NULL);

	for (int i = 0; i < NUM; ++i)
		close(fd[i]);

	return 0;
}
		
