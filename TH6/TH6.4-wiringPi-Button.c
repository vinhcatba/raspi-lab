#include<wiringPi.h>
#include<stdio.h>

#define button 24 //GPIO24
#define led1 5 //GPIO5
#define led2 6 //GPIO6
#define nguon 26 

int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

void bat_led(int led){
	digitalWrite(led,HIGH);
}

void tat_led(int led){
	digitalWrite(led,LOW);
}

int main(void){
	int dem;
	int readbtn;
 	if(wiringPiSetupGpio() ==-1)
	{
		printf("setup wiringPi Failed\n");
		return -1;
	}
 	pinMode(led1,OUTPUT);
 	pinMode(led2,OUTPUT);
 	pinMode(button,INPUT);
 	pinMode(nguon,OUTPUT);
 	digitalWrite(nguon,HIGH);
 	tat_led(led1);
	tat_led(led2);
	while(1){
		readbtn = digitalRead(button);
		// If the switch changed, due to noise or pressing:
		if (readbtn != lastButtonState) {
			// reset the debouncing timer
			lastDebounceTime = millis();
		}
					
		if ((millis() - lastDebounceTime) > debounceDelay) {

			// if the button state has changed:
			if (readbtn != buttonState) {
				buttonState = readbtn;

				if (buttonState == HIGH) {
					dem ++;
				}
			}
		}
		lastButtonState = readbtn;

		if(dem==1){
			bat_led(led1);
			printf("Bat led 1\n");
			
		}
		if(dem==2){
			bat_led(led2);
			printf("Bat led 2\n");
			
		}
		if(dem==3){
			tat_led(led1);
			tat_led(led2);
			printf("Tat ca 2 led\n");
			dem=0;
		}
	}
	return 0;
}
