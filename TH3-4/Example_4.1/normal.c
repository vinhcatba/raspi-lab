#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

#define NUM	5

static char stt[][16] = {"first", "second", "third", "fourth", "fifth"};

int main(int argc, char *argv[]) {
	int ret, fd, id, random;
	char path[64], data[64];

	for (int i = 0; i < NUM; ++i) {
		sprintf(path, "%s-data.txt", stt[i]);
		fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0644);
		if (fd < 0) {
			perror("Could not open file");
			return -1;
		}

		for (int i = 0; i < 50000; i++) {
			srand(clock());
                	random = rand();
        	        id = random % NUM;
	                random = random % 10000;
                	sprintf(data, "%d ", random);

			if (write((fd), &data, strlen(data)) < 0) {
	                        perror("Error from write");
                        	break;
                	}
		}

		close(fd);
	}

	return 0;
}
		
