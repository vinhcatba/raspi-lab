#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>

int listNetInfo(void){
    // danh sách liên kếT chứa thông tin network
    struct ifaddrs *addresses;

    // lấy thông tin network interface cho vào danh sách liên kết 
    if (getifaddrs(&addresses) == -1){
        printf("getifaddrs call failed\n");
        return -1;
    }
    // cấu trúc chứa từng phần tử của ds liên kết
    struct ifaddrs *address = addresses;
    // đi lần lượt từng phần tử trong ds
    while(address){
        // họ giao thức (IPv4 hay IPv6)
        int family = address->ifa_addr->sa_family;
        // chỉ kiểm tra IPv4 hoặc IPv6
        if (family == AF_INET || family == AF_INET6){ 
             // in ra tên interface
            printf("%s\t", address->ifa_name); 
            // nếu family là AF_INET thì in ra "IPv4", không thì in ra "IPv6"
            printf("%s\t", family == AF_INET ? "IPv4" : "IPv6"); 
            char ap[100];
            const int family_size = family == AF_INET ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);
            // chuyển thông tin địa chỉ thành dạng số
            getnameinfo(address->ifa_addr,family_size, ap, sizeof(ap), 0, 0, NI_NUMERICHOST); 
            printf("\t%s\n", ap);
        }
        address = address->ifa_next;
    }
    freeifaddrs(addresses);
    return 0;
}