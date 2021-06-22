đây là 2 ví dụ có trong giáo trình "Lập trình hệ thống nhúng Linux" của cô Vân, thầy Hưởng, học viện Kỹ thuật Mật Mã. Chi tiết xem file .docx

VD1 là tạo chuỗi ngẫu nhiên 32 ký tự mỗi 1s. Chú ý trong giáo trình có `static int main(int argc, char *argv[])` dòng nay cần phải bỏ `static` đi code mới có thể chạy được

VD2 là chương trình xem, sửa thời gian watchdog timer. Chương trình này cần chạy trên Raspberry Pi do các máy linux "không nhúng" sẽ không có wdt (kiểm tra bằng `ls /dev | grep watchdog` nếu không có gì tức là không có wdt)

Một lưu ý nữa là wdt trên raspi 3B đếm đuọc tối đa 16s (từ 15 tới 0), bởi vì thanh ghi wdt timeout trên raspi 3B là 20bit, đếm ngược (-1) mỗi 16 microsecond, tức là 1048576*16*10^-6 = ~16s

![image](https://user-images.githubusercontent.com/29064137/122921310-867aa980-d38c-11eb-9680-459a2e624f59.png)
