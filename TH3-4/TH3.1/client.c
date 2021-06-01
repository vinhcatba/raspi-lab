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
    char head[] = "client";
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

    // map memory voi fd
    addr = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(!addr){
        printf("Failed memory mapping for fd\n");
        goto fail;
    }
    close(fd);

begin:  //BEGIN
    // gui chuoi rong
    strcpy(addr, "");
    msync(addr, BUFF_SIZE, MS_SYNC);

    // doi server ket noi
    printf("Waiting for server...\n");
    while(1){
        // lien tuc kiem tra thong bao cua server
        strcpy(recvbuff, addr);
        // neu co thong bao "server joined" thi phan hoi va break
        if(!strncmp(recvbuff, "server joined", 13)){
            strcpy(addr, "client joined");
            msync(addr, BUFF_SIZE, MS_SYNC);
            printf("server joined\n");
            break;
        }
        
    }
    // bat dau giao tiep
    while(1){
        // doi tin nhan dau tien cua server, nhan dien bang header "server"
        do {
            strcpy(recvbuff, addr);
        }
        while (strncmp("server",recvbuff,6) != 0);
        // neu chuoi la "!exit" thi thong bao disconnect va quay lai begin
        if(!strncmp(recvbuff+6, "!exit", 5)){
            printf("client disconnected\n");
            goto begin;
        }
        // khong thi in ra chuoi
        else{
            printf("Received from server: ");
            printf("%s", recvbuff+6);
        }
        // nhap chuoi tu ban phim va gui 
        printf("input: ");
        fgets(sendbuff, BUFF_SIZE, stdin);
        // chen header "client len dau chuoi truoc khi gui di
        stringWorker();
        strcpy(addr, sendbuff);
        msync(addr, BUFF_SIZE, MS_SYNC);
        // neu chuoi nhap vao la "!exit" thi unmap va thoat
        if(!strncmp(sendbuff+6, "!exit", 5)){
            munmap(addr, PAGE_SIZE);
            return 0;
        }
    }
    munmap(addr, PAGE_SIZE);
    return 0;

fail:
    close(fd);
    return -1;
}


