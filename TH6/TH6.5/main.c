#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	const char *chipname = "gpiochip0";
	struct gpiod_chip *chip;
	struct gpiod_line *line;
	int val;

	// Mở GPIO chip
	chip = gpiod_chip_open_by_name(chipname);

	// Mở GPIO line
	line = gpiod_chip_get_line(chip, 23);
	
	// Mở LED line
	gpiod_line_request_output(line, "TH6.5", 0);

	while(1) {
		gpiod_line_set_value(line, val);
		usleep(500000);
		val = !val;
	}
	gpiod_line_release(line);
	gpiod_chip_close(chip);
	return 0;
}
	
