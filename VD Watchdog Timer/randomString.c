#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static char * rand_string(int len)
{
    static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    char *str = malloc(len + 1);
    int pos;
    for (int i = 0; i < len; ++i) {

    
    srand(clock());
    pos = rand() % strlen(charset);
    str[i] = charset[pos];
    }
    str[len] = '\0';
    return str;
}

int main(int argc, char *argv[])
{
    char *rand_str;
    while (1) {
        rand_str = rand_string(32);
        if (!rand_str)
        return -1;
        printf("%s\n", rand_str);
        free(rand_str);
        sleep(1);
    }
    return 0;
}
