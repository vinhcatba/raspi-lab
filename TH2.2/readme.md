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
Khối `menu/endmenu` sẽ tạo một menu cấu hình trong menuconfig. `config HELLO` sẽ tự thêm tiền tố `CONFIG_` trước `HELLO` nếu module này được chọn trong menuconfig. Lúc này ta được biến `CONFIG_HELLO` như trong `Makefile`. `tristate "hello kernel"` sẽ tạo một lựa chọn "hello kernel" trong menu "hello kernel module". `tristate` nhận 3 giá trị là y, m, n như đã nói ở trên, nếu là `bool` thì nhận 2 giá trị là y hoặc n. `depends on ARM` giúp menu này chỉ hiển thị khi kiến trúc là ARCH=arm. `default M if ARM` cho giá trị mặc định của menu là m nếu kiến trúc là arm. `help` hiển thị trợ giúp menu.
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
