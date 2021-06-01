
#include <stdio.h>
#include <stdlib.h>

// ham worker de ghi 50 trieu so ngau nhien vao file
void worker(char * filename){
    FILE *fd;
    const long repeat = 50000000;
    fd = fopen(filename, "w");
    if(fd == NULL){
        printf("error creating file");
        exit(-1);
    }
    for (long i = 0; i<repeat; i  ++){
        fprintf(fd, "%d", rand());
    }
    close(fd);
}

int main(){
    // goi ham worker 5 lan de ghi vao 5 file khac nhau
    worker("file1");
    worker("file2");
    worker("file3");
    worker("file4");
    worker("file5");
    return 0;
}
