# raspi-lab
Bài tập thực hành lập trình nhúng linux trên raspberry pi, học viện Kỹ thuật Mật Mã 2021.
# Chuẩn bị
Raspberry Pi 3B, được cài Raspbian OS 32bit, một máy host Ubuntu 18.04 hoặc 20.04 (ở đây mình dùng 20.04). Config SSH trên raspi bằng lệnh sudo raspi-config -> vào mục Interface Options -> SSH -> Enable.
# Phần mềm khuyên dùng
- remote.it: tạo kết nối p2p giữa các thiết bị với ras qua internet, có thể chia sẻ quyền với nhiều tài khoản, miễn phí. Sử dụng nó kèm với các phần mềm bên dưới để có thể tạo kết nối ssh, vnc, tcp socket, v.v **qua internet**.
- vnc viewer: remote desktop 
- putty: ssh terminal trên windows
- mobaXterm: terminal ssh, X server, FTP v.v
- VSCode kèm extension SSH: viết code từ máy host lưu trực tiếp trên raspi qua ssh, sử dụng phần cứng của máy host nên không lag so với code trên ras. Phím tắt `` Ctrl + ` ``  để mở terminal
