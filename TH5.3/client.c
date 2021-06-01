#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
 // Chứa cấu trúc cần thiết cho socket.
#include <netinet/in.h>
 // Thư viện chứa các hằng số, cấu trúc khi sử dụng địa chỉ trên internet
#include <arpa/inet.h>
#include <unistd.h>
#define BUFF_SIZE 256
#define handle_error(msg) \
do { perror(msg); exit(EXIT_FAILURE); } while (0)
int main(int argc, char *argv[])
{
    int portno;
    int server_fd;
    struct sockaddr_in serv_addr;
    memset(&serv_addr, '0',sizeof(serv_addr));
    /* Đọc portnumber từ command line */
    if (argc < 3) {
        printf("command : ./client <server address> <port number>\n");

        exit(1);
    }
    portno = atoi(argv[2]);
    /* Khởi tạo địa chỉ server */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) == -1)
        handle_error("inet_pton()");
    /* Tạo socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
        handle_error("socket()");

    /* Kết nối tới server*/
    if (connect(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        handle_error("connect()");
        
    /* Nhận thông điệp từ server bằng hàm read */
    char recvbuff[BUFF_SIZE];
    read(server_fd, recvbuff, sizeof(recvbuff));
    printf("%s\n",recvbuff);

    return 0;
}
