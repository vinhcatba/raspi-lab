#include <stdio.h>
#include <wiringPi.h>
#include <signal.h>
#include <string.h>

#define LED 5

void sig_handler(int s){
    printf("\nSTOPPING\n");
    digitalWrite(LED, 0);
	exit(1); 
}

int main(){
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_handler = sig_handler;
    sigaction(SIGINT,  &act, 0);
    sigaction(SIGTERM, &act, 0);
    signal(SIGTERM, sig_handler);
    
    int state = 0;
    wiringPiSetupGpio();
    pinMode(LED, OUTPUT);
    while (1){
        digitalWrite(LED, state^=1);
        delay(500);
    }
}