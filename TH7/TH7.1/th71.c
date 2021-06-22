#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringSerial.h>

int main() {
	char sendData[50];
	int fd;
	char c;
	memset(sendData, 0, 50);
	if ((fd = serialOpen("/dev/ttyAMA0", 115200)) < 0) {

		fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
	} else {
        // chỉnh sửa cấu hình termios
		struct termios options;
		tcgetattr(fd, &options);
		options.c_lflag |= ICANON;		  // set canonical mode (line by line)
		tcsetattr(fd, TCSANOW | TCSAFLUSH, &options);
        
		while (1) {
			c = 0;
			// cái cụm này là cụm gửi
			printf("\nRaspberry's sending : \n");
			fgets(sendData, 50, stdin);
			serialPuts(fd, sendData);
			serialFlush(fd);
			// cai cum nay la cum nhan
			printf("\nRaspberry's receiving : \n");
			do {
				c = serialGetchar(fd);		  // hàm này lấy tung ky tu 1
				printf("%c", c);
				fflush(stdout);
			} while (serialDataAvail(fd));		  // do while lap lai cho toi khi doc het ky tu
			delay(100);
		}
	}
	close(fd);
	return 0;
}
