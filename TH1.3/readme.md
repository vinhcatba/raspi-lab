# TH1.3: Viết một chương trình C trên máy host, cross-compile để chạy trên raspi
- Trên máy host, cài các công cụ cần thiết:
```shell
sudo apt install git bc bison flex libssl-dev make libc6-dev libncurses5-dev build-essential crossbuild-essential-armhf
```
- Tạo một thư mục làm việc tên raspi:
```shell
mkdir raspi
cd raspi
```
- Trong thư mục raspi tạo một file chương trình `hello_world.c` với nội dung đơn giản như sau:
```c
#include <stdio.h>
int main(){
	printf("hello world!\n");
	return 0;
}
```
- Cross-compile:
```shell
arm-linux-gnueabihf-gcc -o hello hello_world.c
```
- Kết quả ta được một file `hello`, chuyển file đó tới raspberry pi:
```shell
sudo scp <-P <port>> hello pi@<địa chỉ IP>:~/Downloads
```
- SSH tới Pi, chạy thử file `hello` để xem kết quả:
```shell
ssh -l pi <địa chỉ IP> -p <port (mặc định 22)>
cd ~/Downloads
./hello
```
