
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
