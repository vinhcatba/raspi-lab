#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>
#include <time.h>
#define LISTEN_BACKLOG 50
#define BUFF_SIZE 256
#define handle_error(msg) \
do { perror(msg); exit(EXIT_FAILURE); } while (0)
    
int main(int argc, char* argv[])
{
    int port_no, len, opt;
    int server_fd, new_socket_fd;
    struct sockaddr_in serv_addr, client_addr;
    FILE *fp;
    pid_t process_id = 0;
    pid_t sid = 0;
    // 1. Create child process
    process_id = fork();
    // Indication of fork() failure
    if (process_id < 0)
    {
        printf("fork failed!\n");
        // Return failure in exit status
        exit(1);
    }
    // 2. PARENT PROCESS. Need to kill it.
    if (process_id > 0)
    {
        printf("process_id of child process %d \n", process_id);
        // return success in exit status
        exit(0);
    }
    // 3. unmask the file mode
    umask(0);
    // 4. set new session
    sid = setsid();
    if(sid < 0)
    {
        // Return failure
        exit(1);
    }
    // 5. Change the current working directory 
    chdir("/");
    // 6. Close stdin. stdout and stderr
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    
    // 7. Open a log file in append mode.
    fp = fopen ("Log.txt", "a");
    
    // 8. Implement and call some function that does core work for this daemon.
    port_no = 888;
    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    memset(&client_addr, 0, sizeof(struct sockaddr_in));
    /* Tạo socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    /* Khởi tạo địa chỉ cho server */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_no);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //INADDR_ANY

    /* Gắn socket với địa chỉ server */
    bind(server_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    /* Nghe tối đa 5 kết nối trong hàng đợi */
    listen(server_fd, LISTEN_BACKLOG) ;

    /* Dùng để lấy thông tin client */
    len = sizeof(client_addr);
    while (1) 
    {
        new_socket_fd = accept(server_fd, (struct sockaddr*)&client_addr,(socklen_t *)&len);
        char temp[BUFF_SIZE];
        inet_ntop(client_addr.sin_family, (struct sockaddr*)&client_addr, temp, sizeof(temp));
        // tạo mảng sendbuff
        char sendbuff[BUFF_SIZE];
        // chuẩn bị nội dung sendbuff bằng sprintf
        time_t rawtime;
        struct tm * timeinfo;
        time(&rawtime);
        timeinfo = localtime (&rawtime); // lay local time
        // chuan bi buffer va log vao file
        sprintf(sendbuff,"%s connected at time: %s\n", temp, asctime(timeinfo));
        fprintf(fp,"%s connected at time: %s\n", temp, asctime(timeinfo));
        // gửi tới client
        write(new_socket_fd, sendbuff, sizeof(sendbuff));     
        fflush(fp); // force flush ra file
        sleep (1);
    }
    fclose(fp);
    return 0;
    
}
