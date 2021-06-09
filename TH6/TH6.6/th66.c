#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>

static uint64_t epochMilli;
unsigned int millis (void)
{
  uint64_t now ;

#ifdef	OLD_WAY
  struct timeval tv ;

  gettimeofday (&tv, NULL) ;
  now  = (uint64_t)tv.tv_sec * (uint64_t)1000 + (uint64_t)(tv.tv_usec / 1000) ;

#else
  struct  timespec ts ;

  clock_gettime (CLOCK_MONOTONIC_RAW, &ts) ;
  now  = (uint64_t)ts.tv_sec * (uint64_t)1000 + (uint64_t)(ts.tv_nsec / 1000000L) ;
#endif

  return (uint32_t)(now - epochMilli) ;
}

#ifndef CONSUMER
#define CONSUMER	"consumer"
#endif
int buttonState =0;
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

int main(int argc, char **argv)
{
	struct timespec ts ;

  clock_gettime (CLOCK_MONOTONIC_RAW, &ts) ;
  epochMilli = (uint64_t)ts.tv_sec * (uint64_t)1000    + (uint64_t)(ts.tv_nsec / 1000000L) ;
	char *chipname = "gpiochip0";
	unsigned int num1 = 24, num2 = 23; //, num3 = 26;	//GPIO24,5,26
	int val1, val3;
	struct gpiod_chip *chip;
	struct gpiod_line *line1;
	struct gpiod_line *line2;
	//struct gpiod_line *line3;
	int ret1, ret2, ret3;
	int lastState = 0;
	int i = 0;
	chip = gpiod_chip_open_by_name(chipname);
	if (!chip) {
		perror("Open chip failed\n");
		goto end;
	}

	line1 = gpiod_chip_get_line(chip, num1);
	if (!line1) {
		perror("Get line1 failed\n");
		goto close_chip;
	}
	
	line2 = gpiod_chip_get_line(chip, num2);
	if (!line2) {
		perror("Get line2 failed\n");
		goto close_chip;
	}
	
	/*
	line3 = gpiod_chip_get_line(chip, num3);
	if (!line3) {
		perror("Get line3 failed\n");
		goto close_chip;
	}
	*/
	ret1 = gpiod_line_request_input(line1, CONSUMER);
	if (ret1 < 0) {
		perror("Request line1 as input failed\n");
		goto release_line;
	}
	
	ret2 = gpiod_line_request_output(line2, CONSUMER, 0);
	if (ret2 < 0) {
		perror("Request line2 as output failed\n");
		goto release_line;
	}
	/*
	ret3 = gpiod_line_request_output(line3, CONSUMER, 1);
	if (ret3 < 0) {
		perror("Request line3 as output failed\n");
		goto release_line;
	}
	*/
	/* Read input */
	while(1){
		val1 = gpiod_line_get_value(line1);
		//val3 = gpiod_line_get_value(line3);
		printf("Intput %d on line #%u\n", val1, line1);
		if (val1 < 0) {
			perror("Read line input failed\n");
			goto release_line;
		}
		if (val1 != lastState) {
			// reset the debouncing timer
			lastDebounceTime = millis();
		}
		if ((millis() - lastDebounceTime) > debounceDelay) {

			// if the button state has changed:
			if (val1 != buttonState) {
				buttonState = val1;

				if (buttonState == 1) {
					i ++;
				}
			}
		}
		if(i==1){
		ret2 = gpiod_line_set_value(line2, 1);
		}
		if(i==2){
		ret2 = gpiod_line_set_value(line2, 0);
		i=0;
		}
		
		lastState = val1;
}
release_line:
	gpiod_line_release(line1);
	gpiod_line_release(line2);
close_chip:
	gpiod_chip_close(chip);
end:
	return 0;
}




