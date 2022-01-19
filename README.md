# raspi-lab
Bài tập thực hành lập trình nhúng linux trên raspberry pi, học viện Kỹ thuật Mật Mã 2021. Trong mỗi thư mục TH có file giải thích `.docx` + ảnh đề bài, cùng với đó là mã nguồn chương trình. Chú ý đọc readme.md của từng bài TH, nếu có lỗi sai hay lưu ý thì sẽ ghi trong đó. 

- Các bài từ 1 tới 5 là các bài về lập trình C trên linux (shared memory, inter-process comunication, multi-threading, socket, dynamic library,..) đều là những kiến thức và kỹ năng cần thiết nếu sau này muốn làm việc lập trình nói chung và lập trình trên hệ thống linux nói riêng.
- Các bài từ 6 tới 9 là các bài về GPIO và các phần cứng/chức năng cụ thể trên Raspberry/Arduino (như i2c, uart, timer). Đây là những kiến thức cơ bản cho lập trình hệ thống nhúng. 
- Các bài từ 10 trở đi là về cài đặt và sử dụng Qt C++ framework, đây là 1 framework hỗ trợ tạo chương trình có giao diện người dùng (GUI), có thể tạo giao diện bằng cách kéo thả, đa nền tảng với ngôn ngữ C++.
# Chuẩn bị
Raspberry Pi 3B, được cài Raspbian OS 32bit, một máy host Ubuntu 18.04 hoặc 20.04 (ở đây mình dùng 20.04). Config SSH trên raspi bằng lệnh sudo raspi-config -> vào mục Interface Options -> SSH -> Enable.
# Phần mềm khuyên dùng
- remote.it: tạo kết nối p2p giữa các thiết bị với ras qua internet, có thể chia sẻ quyền với nhiều tài khoản, miễn phí. Sử dụng nó kèm với các phần mềm bên dưới để có thể tạo kết nối ssh, vnc, tcp socket, v.v **qua internet**.
- vnc viewer: remote desktop 
- putty: ssh terminal trên windows
- mobaXterm: terminal ssh, X server, FTP v.v
- VSCode kèm extension SSH: viết code từ máy host lưu trực tiếp trên raspi qua ssh, sử dụng phần cứng của máy host nên không lag so với code trên ras. Phím tắt `` Ctrl + ` ``  để mở terminal
