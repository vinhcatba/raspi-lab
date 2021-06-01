#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// ham worker dang pthread de ghi 50 trieu so ngau nhien vao file
void * worker(void * arg){

    char * filename = (char *)arg;
    FILE *fd;
    const long repeat = 50000000;
    fd = fopen(filename, "w");
    if(fd == NULL){
        exit(-1);
    }
    for (long i = 0; i<repeat; i++){
        fprintf(fd, "%d", rand());
    }
    close(fd);
    pthread_exit(NULL);
}

int main(){
    int id = getpid();
    printf("%d\n", id);
    pthread_t thread_id[5];
    char filename[10];
    // tao 5 thread va truyen vao tham so ten filename tang dan
    for(int i=0; i<5; i++){
        sprintf(filename,"filename%d", i+1);
        pthread_create(&thread_id[i], NULL, worker, (void *)filename);
        //pthread_join(thread_id[i], NULL);
        usleep(10000); //sleep 10 milisecond
    }
    pthread_exit(NULL);
}
