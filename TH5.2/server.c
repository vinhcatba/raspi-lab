#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
 // Chứa cấu trúc cần thiết cho socket.
#include <netinet/in.h>
 // Thư viện chứa các hằng số, cấu trúc khi sử dụng địa chỉ trên internet
#include <unistd.h>
#include <arpa/inet.h>
#include <wiringPi.h>
#include <pthread.h>

#define LED_pin 18

#define LISTEN_BACKLOG 50
#define BUFF_SIZE 256
#define handle_error(msg) \
do { perror(msg); exit(EXIT_FAILURE); } while (0)

pthread_t thread_id = NULL;

// thread de dieu khien PWM LED, chay song song voi giao tiep socket
void * pwmLed(void *arg){
    pinMode (LED_pin, PWM_OUTPUT) ; /* set PWM pin as output */
    printf("PWM Start\n");
    int intensity ;  
    while (1)
    {
        
        for (intensity = 0 ; intensity < 1024 ; ++intensity)
        {
            pwmWrite (LED_pin, intensity) ;	/* provide PWM value for duty cycle */
            delay (2) ;
        }
        delay(2);

        for (intensity = 1023 ; intensity >= 0 ; --intensity)
        {
            pwmWrite (LED_pin, intensity) ;
            delay (2) ;
        }
        delay(2);
    }
    pthread_exit(NULL);	
}


/* Chức năng chat */
void chat_func(int new_socket_fd)
{
    int numb_read, numb_write;
    char sendbuff[BUFF_SIZE];
    char recvbuff[BUFF_SIZE];
    while (1) {
        memset(sendbuff, '0', BUFF_SIZE);
        memset(recvbuff, '0', BUFF_SIZE);
        /* Đọc dữ liệu từ socket */
        /* Hàm read sẽ block cho đến khi đọc được dữ liệu */
        numb_read = read(new_socket_fd, recvbuff, BUFF_SIZE);
        if(numb_read == -1)
            handle_error("read()");
        if (strncmp("exit", recvbuff, 4) == 0) {
            system("clear");
            break;
        }
        if (strncmp("on", recvbuff, 2) == 0) {
            printf("Led ON\n");
            /// GPIO PWM
            if(thread_id == NULL){
                pthread_create(&thread_id, NULL, pwmLed, NULL);
                strcpy(sendbuff, "LED da bat");
            }
        }else if (strncmp("off", recvbuff, 3) == 0) {
            printf("Led OFF\n");
            /// GPIO OFF
            pthread_cancel(thread_id);
            pinMode(LED_pin, OUTPUT);
            digitalWrite(LED_pin, 0);
            thread_id = NULL;
            strcpy(sendbuff, "LED da tat");
        }

        /* Nhập phản hồi từ bàn phím */
        // printf("Please respond the message : ");
        // fgets(sendbuff, BUFF_SIZE, stdin);

        /* Ghi dữ liệu tới client thông qua hàm write */
        numb_write = write(new_socket_fd, sendbuff, sizeof(sendbuff));
        if (numb_write == -1)
            handle_error("write()");
        if (strncmp("exit", sendbuff, 4) == 0) {
            system("clear");
            break;
        }
        sleep(1);
    }
    close(new_socket_fd);
}
int main(int argc, char *argv[])
{
    int port_no, len, opt;
    int server_fd, new_socket_fd;
    struct sockaddr_in serv_addr, client_addr;
    /* Đọc portnumber trên command line */
    if (argc < 2) {
        printf("No port provided\ncommand: ./server <port number>\n");
        exit(EXIT_FAILURE);
    } else
        port_no = atoi(argv[1]);
    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    memset(&client_addr, 0, sizeof(struct sockaddr_in));
    /* Tạo socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
        handle_error("socket()");
    // fprintf(stderr, "ERROR on socket() : %s\n", strerror(errno));
    /* Ngăn lỗi : “address already in use” */
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
        handle_error("setsockopt()");

    /* Khởi tạo địa chỉ cho server */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_no);
    serv_addr.sin_addr.s_addr = INADDR_ANY; //dung INADD_ANY de ket noi tu may khac cung mang

    /* Gắn socket với địa chỉ server */
    if (bind(server_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        handle_error("bind()");
    /* Nghe tối đa 5 kết nối trong hàng đợi */
    if (listen(server_fd, LISTEN_BACKLOG) == -1)
        handle_error("listen()");
    /* Dùng để lấy thông tin client */
    len = sizeof(client_addr);

    wiringPiSetupGpio();
    pinMode (LED_pin, OUTPUT);
    while (1) {
        printf("Server is listening at port : %d \n....\n",port_no);
        new_socket_fd = accept(server_fd, (struct sockaddr*)&client_addr, (socklen_t *)&len);
        if (new_socket_fd == -1)
            handle_error("accept()");

        system("clear");
        char temp[BUFF_SIZE];
        inet_ntop(client_addr.sin_family, (struct sockaddr*)&client_addr, temp, sizeof(temp));
        printf("Server : got connection from %s\n", temp);
        // LED nhay 3 lan 
        for (int i = 0; i<3; i++){
                digitalWrite(LED_pin, 1);
                delay(150);
                digitalWrite(LED_pin, 0);
                delay(150);
        }
        chat_func(new_socket_fd);
    }
    close(server_fd);
    return 0;
}
