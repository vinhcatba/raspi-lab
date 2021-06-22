#include <i2c1602.h>
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#define LCD_ADDR 0x3f		 // địa chỉ của lcd i2c, dùng lệnh "i2cdetect -y 1" để xem địa chỉ
I2C16x2 lcd;				 // tạo struct lcd chứa địa chỉ I2C

int main() {
	wiringPiSetup();		   // gọi wiringPiSetup để lấy thời gian cho các hàm delay và delayMicroseconds
	lcd_init(LCD_ADDR);		   // khởi tạo lcd tại địa chỉ LCD_ADDR, xoá LCD RAM sau khi khởi tạo
							   // có thể dùng hàm lcd_setAddr(LCD_ADDR) để đổi địa chỉ LCD mà không cần init lại
	ClrLcd();

	while (1) {

		int number = 123;
		float Pi = 3.14;
		ClrLcd();						  // xoá màn LCD, xoá RAM trên LCD
		typeChar('V');					  // in chữ 'V'
		lcdLoc(LINE1 + 4);				  // đặt con trỏ ở hàng 1 cột 4
		typeString("Hello World");		  // in ra chuỗi "hello"
		lcdLoc(LINE2);					  // đặt con trỏ ở hàng 2 cột 0
		typeInt(number);				  // in số nguyên number
		lcdLoc(LINE2 + 6);
		typeFloat(Pi);		  // in ra số Pi
		delay(2000);
		lcd_byte(0x1c, LCD_CMD);		// Gửi lệnh 0x1c (dịch toàn bộ màn hình sang phải 1 cột).
		delay(2000);
	}
	return 0;
}
