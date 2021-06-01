#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/mman.h>

#define FILE "./data.txt"   // ten file
#define PAGE_SIZE 4096      // kich thuoc mapping
#define BUFF_SIZE 256       // kich thuoc buffer

char sendbuff[BUFF_SIZE];   // buffer gui
char recvbuff[BUFF_SIZE];   // buffer nhan

// ham chen header "server" vao dau buffer
void stringWorker(){
    char head[] = "server";
    char tail[BUFF_SIZE];
    strcpy(tail, sendbuff);
    strcat(head, tail);
    strcpy(sendbuff, head);
}

int main(){
    int fd, length, ret;
    char *addr = NULL;
    // mo file
    fd = open(FILE, O_RDWR | O_CREAT, 0640);
    if(fd < 0){
        printf("Cannot open file\n");
        return -1;
    }
    // doi kich thuoc file
    ret = ftruncate(fd, PAGE_SIZE);
    if(ret){
        printf("Failed changing file size\n");
        goto fail;
    }

    //memory mapping voi flag la shared
    addr = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(!addr){
        printf("Failed memory mapping for fd\n");
        goto fail;
    }
    close(fd);

begin: //BEGIN
    // gui chuoi trong
    strcpy(addr, "");
    msync(addr, BUFF_SIZE, MS_SYNC);

    // doi client ket noi
    printf("Waiting for client...\n");
    while(1){
        // lien tuc gui di chuoi thong bao
        strcpy(addr, "server joined");
        msync(addr, BUFF_SIZE, MS_SYNC);
        // kiem tra phan hoi cua client
        strcpy(recvbuff, addr);
        if(!strncmp(recvbuff, "client joined", 13)){
            printf("client joined\n");
            break;
        }
    }
    // ket noi thanh cong, thuc hien giao tiep
    int dem=0;
    char *endptr;
    while(1){
        
        sprintf(sendbuff,"%d",dem);
        //chen header "server" len dau chuoi
        stringWorker();
        //gui di
        strcpy(addr, sendbuff);
        msync(addr, BUFF_SIZE, MS_SYNC);
        sleep(1);
        //neu chuoi la "!exit" ("server!exit") thi unmap va ket thuc 
        if(!strncmp(sendbuff+6, "!exit", 5)){
            munmap(addr, PAGE_SIZE);
            return 0;
        }
        
        // lien tuc kiem tra xem client da phan hoi chua bang header "client"
        do {
            strcpy(recvbuff, addr);
        }
        while (strncmp("client",recvbuff,6) != 0);
        // neu chuoi phan hoi cua client la "!exit" thi thong bao ngat ket noi va quay lai begin
        if(!strncmp(recvbuff+6, "!exit", 5)){
            printf("client disconnected\n");
            goto begin;
        }
        // in ra chuoi phan hoi cua client
        else{
            printf("Received from client: ");
            printf("%s\n", recvbuff+6);
            // tang bien dem
            dem = strtol(recvbuff+6,&endptr,10);
            dem++;
        }
    }
    
    munmap(addr, PAGE_SIZE);
    return 0;

fail:
    close(fd);
    return -1;
}


