#include <wiringPi.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int led[] = {0, 5, 6, 13, 19, 26};

void sig_handler(int s){
    printf("\nSTOPPING\n");
	for (int i=0; i<6; i++){
        digitalWrite(led[i], 0);
    }
	exit(1); 
}

int main(){
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_handler = sig_handler;
    sigaction(SIGINT,  &act, 0);
    sigaction(SIGTERM, &act, 0);

    wiringPiSetupGpio();
    for (int i=0; i<6; i++){
        pinMode(led[i], OUTPUT);
        digitalWrite(led[i], 0);
    }
    while(1){
        for (int i = 0; i<5; i++){
            digitalWrite(led[i], 1);
            delay(100);
            digitalWrite(led[i], 0);
            //delay(100);
        }
        for (int i = 5; i>0; i--){
            digitalWrite(led[i], 1);
            delay(100);
            digitalWrite(led[i], 0);
            
        }
    }
    return 0;

}
