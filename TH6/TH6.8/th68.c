#include <stdio.h>
#include <gpiod.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#define RX_OFFSET 5 // rx pin
#define TX_OFFSET 6 // tx pin

#define BUFF_SIZE 256
#define NSEC_BIT_TIME 104000 // thờI gian của 1 bit theo nanosecon, tính theo baud rate 9600bit/s


/* chip và line global */
struct gpiod_chip *chip;
struct gpiod_line *RXline, *TXline;

pthread_mutex_t lock;

/* lấy thời gian hiện tại theo nanosecond */
uint64_t getnsec(){
    struct  timespec ts ;
    uint64_t now ;
    clock_gettime (CLOCK_MONOTONIC_RAW, &ts) ;
    now  = (uint64_t)ts.tv_sec * (uint64_t)1000000000 + (uint64_t)(ts.tv_nsec) ;
    return (uint64_t)now;
}

/* sử dụng vòng lặp while để delay 1 khoảng thờI gian bằng nsec (nanosecond) */
void waitnsec(uint64_t nsec){
    uint64_t last ;
    last = getnsec();
    while((getnsec() - last) < nsec);
}

/* thread để bắt dữ liệu RX */
void *CatchRX(void * arg){
    int charCount = 0;      // đếm số byte
    char RXbuffer;   // 1 byte nhận được
    /* dùng vòng lặp while(1) để liên tục check start bit */
    while(1){
        RXbuffer = 0x00;   
        if(gpiod_line_get_value(RXline) == 0){  // nếu RXline = 0 => start bit low => bắt đầu đọc dữ liệu
            pthread_mutex_lock(&lock);
            if (charCount == 0){
                printf("\nChuoi nhan duoc: ");
            }
            waitnsec(NSEC_BIT_TIME);    // đang ở start bit, đợi NSEC_BIT_TIME để đọc bit đầu tiên (LSD - Least Significant Bit)
            /* Đọc 8 lần = 8 bit = 1 byte */
            for(int i=0; i<8; i++){
                RXbuffer |=  (gpiod_line_get_value(RXline)<<i) ; // ghi từng bit vào RXbuffer bằng bitwise OR, vì tín hiệu là LSD trước nên phải dịch trái để ghi bit từ phải sang
                waitnsec(NSEC_BIT_TIME);    // đợi
            }
            printf("%c", RXbuffer); // in byte vừa đọc ra console
            charCount++;
            fflush(stdout); //force flush vì không có '\n'
            if (RXbuffer == '\n'){
                printf("Nhap chuoi can gui: ");
                fflush(stdout);
                charCount = 0;
            }
            pthread_mutex_unlock(&lock);
        }
    }
    pthread_exit(NULL);
}

/* Hàm này gửi 1 byte qua tx */
void sendByte(uint8_t byteBuff){
    pthread_mutex_lock(&lock);
    gpiod_line_set_value(TXline, 0); // start bit là LOW
    waitnsec(NSEC_BIT_TIME);    // đợi
    /* Gửi 8 bit đi */
    for(int i=0; i<8; i++){
        gpiod_line_set_value(TXline, (byteBuff>>i)&0x01); // Gửi LSB trước, dịch phải i bit và dùng bitwise AND để lấy bit đầu tiên từ bên phải tức là bit LSB
        waitnsec(NSEC_BIT_TIME);
    }
    gpiod_line_set_value(TXline, 1); // stop bit là HIGH
    waitnsec(NSEC_BIT_TIME);
    pthread_mutex_unlock(&lock);
}

/* Hàm gửi 1 chuỗi, là 1 wrapper quanh hàng sendByte() */
void sendString(uint8_t *str, int length){
    uint8_t byteBuff;
    /* đi từng ký tự một trong chuỗi */
    for(int i=0; i<length; i++){
        strncpy(&byteBuff, str+i, 1);   // lấy 1 ký tự để truyền vào sendByte() 
        sendByte(byteBuff);
    }
}

int main(){
    pthread_t tid;
    /* Khởi tạo line, RX là INPUT, TX là OUTPUT, TX luôn để HIGH */
    chip = gpiod_chip_open_by_number(0);
    RXline = gpiod_chip_get_line(chip, RX_OFFSET); 
    TXline = gpiod_chip_get_line(chip, TX_OFFSET); 
    gpiod_line_request_input(RXline, "RX");
    gpiod_line_request_output(TXline, "TX", 1);

    /* init mutex */
    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("\n mutex init has failed\n");
        return 1;
    }
    
    /* tạo rx thread */
    pthread_create(&tid, NULL, CatchRX, NULL);

    /* thực hiện gửi chuỗi 10 lần */
    int t = 10;
    while(t--){
        char str[BUFF_SIZE];
        printf("Nhap chuoi can gui: ");
        fgets(str, BUFF_SIZE, stdin);
        sendString(str, strlen(str));
    } 

    /* đóng thread */
    pthread_mutex_destroy(&lock);
    pthread_cancel(tid);

    /* đóng line, chip */
    gpiod_line_release(RXline);
    gpiod_line_release(TXline);
    gpiod_chip_close(chip);
    return 0;
}