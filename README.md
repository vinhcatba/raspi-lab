# raspi-lab
Bài tập thực hành lập trình nhúng linux trên raspberry pi
# Chuẩn bị
Raspberry Pi 3B, được cài Raspbian OS 32bit, một máy host Ubuntu 18.04 hoặc 20.04 (ở đây mình dùng 20.04). Config SSH trên raspi bằng lệnh sudo raspi-config -> vào mục Interface Options -> SSH -> Enable.
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
# TH1.4 Triển khai linux kernel cho raspberry pi
Mục tiêu của bâì thực hành này là để cài đặt hệ điều hành Linux cho Raspberry Pi, tuy nhiên nhóm đã và quyết định dùng hệ điều hành Raspbian. Vì thế nên bài này nhóm chỉ thực hiện build kernel trên máy host.
Nguồn tham khảo chính: https://www.raspberrypi.org/documentation/linux/kernel/building.md 

Hầu hết các tool cần thiết đã được cài ở TH1.3, nên chúng ta bắt đầu bằng việc clone nguồn.
> Để thuận tiện cho các bài lab sau này, cần chú ý clone đúng nhánh trùng với phiên bản mà raspbian đang sử dụng. Kiểm tra trên Raspberry Pi bằng lệnh:
> ```shell
> apt search raspberrypi-kernel
> ```
> Ta sẽ thấy dòng `raspberrypi-kernel/testing,now 1.20210430-1 armhf [installed]` trong đó `1.20210430-1` chính là tag phiên bản kernel hiện tại. 
> Duyệt tìm tag đó ở https://github.com/raspberrypi/linux sẽ có kết quả là `raspberrypi-kernel_1.20210430-1` tại: https://github.com/raspberrypi/linux/tree/raspberrypi-kernel_1.20210430-1
> ![image](https://user-images.githubusercontent.com/29064137/117937314-080ffe00-b330-11eb-85ef-83bf4e5f1310.png)
> 
> Để cho chắc chắn, ta nên kiểm tra cả số phiên bản kernel-release. Trên Raspberry Pi chạy lệnh `uname -r` để xem. Kể quả có dạng: `5.10.17-v7+`
> Vào github vừa tìm được (https://github.com/raspberrypi/linux/tree/raspberrypi-kernel_1.20210430-1) mờ Makefile ở 5 dòng đầu ta có thể thấy:
> ```makefile
> VERSION = 5
> PATCHLEVEL = 10
> SUBLEVEL = 17
> ```
> tương đương với `5.10.17`, tức là phiên bản trùng khớp với phiên bản trên Raspberry Pi.
> Nếu phiên bản trên Raspberry Pi có `SUBLEVEL` lớn hơn (VD `5.10.35` có `SUBLEVEL` là 35 > 17), hoặc nếu trước đó Raspberry Pi đã chạy lệnh `sudo rpi-update` thì có thể hạ xuống bằng lệnh:
> ```shell
> sudo apt update
> sudo apt install --reinstall raspberrypi-bootloader raspberrypi-kernel
> sudo reboot
> ```
> Và thực hiện kiểm tra phiên bản lại.

- Sau khi tìm được phiên bản chính xác, clone nó trên máy host:
```shell
cd raspi
git clone --depth=1 --branch raspberrypi-kernel_1.20210430-1 https://github.com/raspberrypi/linux.git
```
- Sau khi hoàn tất clone, sẽ có thư mục `linux`, chuyển đến thư mục này và chạy các lệnh sau để tạo config 32 bit cho Raspberry Pi 3:
```shell
cd linux
KERNEL=kernel7
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- bcm2709_defconfig
```
- Build kernel theo những config đó, trong đó `-j $(($(nproc)*3/2))` là đoạn code nhỏ để build nhanh hơn. Tham số `-j n` chia nhỏ công việc thành `n` tác vụ, `nproc` sẽ cho kết quả là số nhân của CPU. Tóm lại, `-j $(($(nproc)*3/2))` sẽ chia nhỏ công việc thành `n = <số nhân CPU> * 1.5` tác vụ để build nhanh hơn
```shell
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- zImage modules dtbs -j $(($(nproc)*3/2))
```
- Các bước tiếp theo, là cài đặt nhân vào thẻ nhớ, có thể tham khảo tiếp trong https://www.raspberrypi.org/documentation/linux/kernel/building.md

# TH2.1 Cải tiến, sửa đổi mã nguồn nhân
**Thao khảo trang 73 giáo trình.**
Từ TH1.4, ta đã có mã nguồn nhân tại thư mục raspi/linux.
- Di chuyển tới thư mục drivers trong thư mục mã nguồn, tạo một thư mục mới ở đây. Thư mục mới này sẽ là thư mục chứa module tùy chỉnh.
```shell
cd raspi/linux
mkdir hello
cd hello
```
- Trong thư mục `hello` sẽ có 3 file là: file mã nguồn .c, Makefile và Kconfig
- Tạo file `hello.c` với nội dung:
```c
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>

static int mod_init(void)
{
    printk(KERN_ALERT "Hello world\n");
return 0;
}
static void mod_exit(void)
{
    printk(KERN_ALERT "Goodbye\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("thanhvinhkma");
MODULE_DESCRIPTION("Custom Linux kernel module");

module_init(mod_init);
module_exit(mod_exit);
```
- Tạo file `Makefile` với nội dung:
```Makefile
EXTRA_CFLAGS = -Wall
obj-$(CONFIG_HELLO) += hello.o
```
`CONFIG_HELLO_WORLD` có thể là y hoặc m tùy theo lựa chọn trong config. y là biên dịnh built-in, m là biên dịch theo kiểu module, n là bỏ qua không biên dịch.
- Tạo file `Kconfig` với nội dung:
```Makefile
menu "hello kernel module"

config HELLO
    tristate "hello kernel"
    depends on ARM
    default y if ARM
    help
        Select this to run hello kernel module
endmenu
```
Khối `menu/endmenu` sẽ tạo một menu cấu hình trong menuconfig. `config HELLO` sẽ tự thêm tiền tố `CONFIG_` trước `HELLO` nếu module này được chọn trong menuconfig. Lúc này ta được biến `CONFIG_HELLO` như trong `Makefile`. `tristate "hello kernel"` sẽ tạo một lựa chọn "hello kernel" trong menu "hello kernel module". `tristate` nhận 3 giá trị là y, m, n như đã nói ở trên, `bool` nhận 2 giá trị là y hoặc n. `depends on ARM` giúp menu này chỉ hiển thị khi kiến trúc là ARCH=arm. `default M if ARM` cho giá trị mặc định của menu là m nếu kiến trúc là arm. `help` hiển thị trợ giúp menu.
- Tiếp đó, ta cần thêm đường dẫn tới 2 file `Makefile` và `Kconfig` mới tạo để hệ thống biết. ***Chuyển tới thư mục mẹ `drivers`*** và chỉnh sửa `Makefile`, `Kconfig` ở đây:
    - Trong file Kconfig thêm dòng như sau:
`source “drivers/hello/Kconfig”`
    - Trong file Makefile thêm dòng như sau:
`obj-y += hello/`
- Chạy menuconfig, vào menu "Device Drivers" để xem kết quả, ta sẽ thấy menu "hello kernel module"
```shell
make ARCH=arm menuconfig
```
![image](https://user-images.githubusercontent.com/29064137/117946208-48c04500-b339-11eb-91b7-1eeb036e0e35.png)
![image](https://user-images.githubusercontent.com/29064137/117946313-64c3e680-b339-11eb-9c8d-fb07f2f89d85.png)
- Sau đó Save và Exit menuconfig
- Build lại kernel và ta sẽ có một kernel với module hello được cài sẵn.
```shell
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- zImage modules dtbs -j $(($(nproc)*3/2))
```
- Tiếp theo, cài đặt nhân vào thẻ nhớ tham khảo trong https://www.raspberrypi.org/documentation/linux/kernel/building.md
# TH2.2 Thực hiện cross-compile module kernel
Đối với bài này, nhóm sử dụng module hello từ TH2.1, nhưng thay vì build nó kèm với kernel dưới dạng built-in thì nhóm thực hiện compile như một module riêng lẻ.
- Tạo một thư mục làm việc **trên máy host**:
```shell
mkdir hello_module
cd hello_module
```
- Tạo file `hello_module.c` với nội dung tương tự như module hello của TH2.1:
```c
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>

static int mod_init(void)
{
    printk(KERN_ALERT "Hello world\n");
return 0;
}
static void mod_exit(void)
{
    printk(KERN_ALERT "Goodbye\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("thanhvinhkma");
MODULE_DESCRIPTION("Custom Linux kernel module");

module_init(mod_init);
module_exit(mod_exit);
```
- Tạo `Makefile` với nội dung:
```makefile
# obj-m biên dịch theo kiểu module
obj-m += hello_module.o

# Thư mục hiện tại
PWD  := $(shell pwd)

# kiến trúc target
ARCH := arm

# Thư mục chứa mã nguồn kernel (từ TH1.4)
KDIR := /mnt/ntfs/raspi/linux

# cross-compiler
CROSS_COMPILE := arm-linux-gnueabihf-

pi:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
```
- Cross-compile:
```shell
make pi
```
- Gửi file `hello_module` tới Raspberry Pi qua SSH:
```shell
sudo scp <-P <port>> hello_module.ko pi@<địa chỉ IP>:~/Downloads
```
- Trên Raspberry Pi thực hiện Insert module, Remove module và `dmesg` để xem kết quả:
```shell
cd ~/Downloads
sudo insmod hello_module.ko
sudo rmmod hello_module
dmesg
```


