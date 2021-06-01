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
