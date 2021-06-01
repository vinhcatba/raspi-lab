#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <linux/watchdog.h>


#define TIMEOUT 1 
#define FILE "/dev/watchdog1"

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static int timeout = TIMEOUT;
static int time_wdt;
static int fd = -1;

void *ping_wdt(void *args)
    {
    int ret;
    while(1) {
        pthread_mutex_lock(&mutex);
        if(timeout-- <= 2) {
            ret = ioctl(fd, WDIOC_KEEPALIVE, 0);
            if(ret) {
                pthread_mutex_unlock(&mutex);
                pthread_exit(NULL);
            }
            ret = ioctl(fd, WDIOC_GETTIMEOUT, &timeout);
            if(ret) {
                pthread_mutex_unlock(&mutex);
                pthread_exit(NULL);
            }
        }
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    pthread_exit(NULL);
}
int main()
{
    int ret, cs, len;
    pthread_t tid;
    fd = open(FILE, O_RDWR);
    if(fd < 0) {
        printf("Counld not open file\n");
        exit(1);
    }
    ret = pthread_create(&tid, NULL, ping_wdt, NULL);
    if(ret) {
        printf("The thread registered failed\n");
        goto out;
    }
    while(1) {
        printf("\t---Watchdog---\n");
        printf("1: Setting timeout.\n");
        printf("2: Getting timeout.\n");
        printf("3: Getting timeleft\n");
        printf("0: Exit.\n");
        scanf("%d", &cs);
        switch(cs) {
            case 1:
                printf("Enter timeout: ");
                scanf("%d", &time_wdt);
                pthread_mutex_lock(&mutex);
                timeout = time_wdt;
                ret = ioctl(fd, WDIOC_SETTIMEOUT, &timeout);
                if(ret) {
                    printf("Setting the timeout failed\n");
                    pthread_mutex_unlock(&mutex);
                    break;
                }
                printf("The timeout was set to %d seconds\n", timeout);
                pthread_mutex_unlock(&mutex);
                break;
            case 2:
                ret = ioctl(fd, WDIOC_GETTIMEOUT, &time_wdt);
                if(ret) {
                    printf("Getting the timeout failed\n");
                    break;
                }
                printf("Timeout is %d seconds\n", time_wdt);
                break;
            case 3:
                ret = ioctl(fd, WDIOC_GETTIMELEFT, &time_wdt);
                if(ret) {
                    printf("Getting the timeleft failed\n");
                    break;
                }
                printf("The timeleft is %d seconds\n", time_wdt);
                break;
            case 0:
                goto out;
            default:
                printf("The option is invalid\n");
                break;
        }
    }
out:
    len = write(fd, "V", sizeof("V"));
    if(len < 0)
    goto out;
    close(fd);
    return ret;
}
