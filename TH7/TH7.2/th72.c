#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <wiringSerial.h>

#define SERIAL_DEV "/dev/ttyAMA0"
#define BUFF_SIZE 256

int fd;
pthread_t tid[2];
pthread_mutex_t lock;

// thread rx
void *rx(void *arg) {
	char receivStr[BUFF_SIZE];
	char c;
	while (1) {
		while (serialDataAvail(fd)) {
			pthread_mutex_lock(&lock);
			c = serialGetchar(fd);
			pthread_mutex_unlock(&lock);
			strncat(receivStr, &c, 1);
		}
		if (strlen(receivStr)) {
			printf("\33[A\33[2K\rTin vua nhan: %s", receivStr);
			printf("\33[2K--------------\nNhap tin: ");
			fflush(stdout);
		}
		memset(receivStr, 0, BUFF_SIZE);
		sleep(1);
	}
}

// thread tx
void *tx(void *arg) {

	char sendStr[BUFF_SIZE];
	while (1) {
		printf("\33[2K--------------\nNhap tin: ");
		fflush(stdout);
		fgets(sendStr, BUFF_SIZE, stdin);

		pthread_mutex_lock(&lock);
		serialPuts(fd, sendStr);
		pthread_mutex_unlock(&lock);
		printf("\33[2A\33[2KT\rTin vua gui: %s", sendStr);
		//printf("\33[2KNhap tin nhan: ");
		fflush(stdout);

		usleep(100);
	}
}

// hàm finish đóng các thread
void finish() {
	pthread_cancel(tid[0]);
	pthread_cancel(tid[1]);
	pthread_mutex_destroy(&lock);
	serialClose(fd);
}

// signal handler bắt tín hiệu Ctrl + C
void sigHandler(int sig) {
	if (sig == SIGINT) {
		finish();
	}
}

int main() {
	signal(SIGINT, sigHandler);

	if ((fd = serialOpen(SERIAL_DEV, 115200)) < 0) {
		printf("Khong the mo port " SERIAL_DEV "\n");
	} else {
		struct termios config;
		tcgetattr(fd, &config);
		config.c_lflag |= ICANON;		 // line by line input mode
		tcsetattr(fd, TCSANOW, &config);

		pthread_mutex_init(&lock, NULL);

		pthread_create(&tid[0], NULL, rx, NULL);
		pthread_create(&tid[1], NULL, tx, NULL);
	}
	finish();
	pthread_exit(NULL);
}
