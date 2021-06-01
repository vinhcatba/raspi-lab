#include <stdio.h>
#include "listnetinfo.h"

int main(){
    int ret;
    puts("This is a shared library test...");
    ret = listNetInfo();
    return ret;
}
// author: Le Thanh Vinh
