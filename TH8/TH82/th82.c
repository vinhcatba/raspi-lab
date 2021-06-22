#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#define I2C_ADDR 0x08		 // địa chỉ i2c của arduino
#define BUFF_SIZE 100		 // kích thước buffer

int main() {
	wiringPiSetup();
	int fd = wiringPiI2CSetup(I2C_ADDR);				   // mở file i2c
	char sendStr[BUFF_SIZE], receiveStr[BUFF_SIZE];		   // khai báo các buffer

	while (1) {
		memset(receiveStr, 0, BUFF_SIZE);		 // clear buffer
		memset(sendStr, 0, BUFF_SIZE);			 // clear buffer
		printf("Input: ");
		fgets(sendStr, BUFF_SIZE, stdin);		 // nhập chuỗi từ bàn phím

		// bởi vì thư viện wiringPi i2c chỉ hỗ trợ gửi từng byte một
		// nên cần dùng vòng lặp để gửi lần lượt từng ký tự trong chuỗi
		// cho tới khi gặp ký tự NULL ('\0') kết thúc chuỗi
		int i = 0;
		while (sendStr[i] != '\0') {
			wiringPiI2CWrite(fd, sendStr[i]);
			i++;
			if (sendStr[i] == '\0') {
				wiringPiI2CWrite(fd, '\0');
			}
		}

		// đọc dữ liệu từ i2c (nếu có). Thư viện wiringPiI2C chỉ chỗ trợ đọc 1 byte
		// nên ở đây dùng read()
		read(fd, receiveStr, BUFF_SIZE);
		printf("%s", receiveStr);
		fflush(stdout);
		usleep(100);
	}
	return 0;
}