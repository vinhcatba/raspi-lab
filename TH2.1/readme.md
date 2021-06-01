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
> 
> ![image](https://user-images.githubusercontent.com/29064137/117937314-080ffe00-b330-11eb-85ef-83bf4e5f1310.png)
> 
> Để cho chắc chắn, ta nên kiểm tra cả số phiên bản kernel-release. Trên Raspberry Pi chạy lệnh `uname -r` để xem. Kể quả có dạng: `5.10.17-v7+`
> Vào github vừa tìm được (https://github.com/raspberrypi/linux/tree/raspberrypi-kernel_1.20210430-1) mở Makefile tại thư mục ngoài cùng. Ở những dòng đầu ta có thể thấy:
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
