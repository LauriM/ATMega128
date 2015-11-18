#define	true	1
#define false	0

#define	NB_SHOTS	5
#define STEP_MAX	1

#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include "lcd.h"
#include "joystick.h"

int main(void)
{
	// IO-port INIT
	DDRA = 0xE0;

	lcd_init();
	
	//TODO: move this to init
	lcd_clear();

	int x = 0;

	for(;;)
	{
		lcd_pixel(7,5);
		lcd_pixel(7,6);
		lcd_pixel(x,7);
		lcd_pixel(x,8);
		lcd_pixel(x,9);
		lcd_pixel(x,10);
		lcd_pixel(x,11);

		_delay_ms(500);

		x++;

		if(x > 128)
			x = 1;

		lcd_display();
	}
}
