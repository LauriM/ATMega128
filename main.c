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

	lcd_clear();

	lcd_goto_xy_exact(5, 3);

	//test loop
	int ticker = 0;
	
	// new "modern" loop
	
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
	


// old test loop
	for(;;)
	{
		if(ticker == 0)
			ticker = 1;
		else
			ticker = 0;

		_delay_ms(150);

//--- {Enabling controllers}
		// Data/DC are outputs for the lcd (all low)
		LCD_DDR |= LCD_DATA_PIN | LCD_DC_PIN;
	    // Enable display controller (active low)
	    LCD_PORT &= ~LCD_CE_PIN;	
//----

		// DATA MODE ON 
		LCD_PORT |= LCD_DC_PIN;
		// COMMAND MODE 
		//LCD_PORT &= ~LCD_DC_PIN;

//########################################


		//ticker test flash draw
		for(int q = 0; q < 500; ++q)
		{
				for(unsigned char i=0;i<8;i++) {
					// on or off
					if(ticker == 1)
						LCD_PORT |= LCD_DATA_PIN;
					else
						LCD_PORT &= ~LCD_DATA_PIN;

					// Toggle the clock
					LCD_PORT |= LCD_CLK_PIN;
					for(int j=0;j<10;j++); // lisätty pientä viivettä
					LCD_PORT &= ~LCD_CLK_PIN;
				}
		}
/*
		//clear
		for(int q = 0; q < 2000; ++q)
		{
				for(unsigned char i=0;i<8;i++) {
					LCD_PORT &= ~LCD_DATA_PIN;

					// Toggle the clock
					LCD_PORT |= LCD_CLK_PIN;
					for(int j=0;j<10;j++); // lisätty pientä viivettä
					LCD_PORT &= ~LCD_CLK_PIN;
				}
		}

		for(int q = 0; q < 500; ++q)
		{
				for(unsigned char i=0;i<8;i++) {
					// on or off
					if(i < 7)
					{
						if(ticker == 1)
							LCD_PORT |= LCD_DATA_PIN;
						else
							LCD_PORT &= ~LCD_DATA_PIN;
					}
					else
						LCD_PORT &= ~LCD_DATA_PIN;

					// Toggle the clock
					LCD_PORT |= LCD_CLK_PIN;
					for(int j=0;j<10;j++); // lisätty pientä viivettä
					LCD_PORT &= ~LCD_CLK_PIN;
				}
		}
*/
//########################################

		//---- {Disabling controllers}

		// Disable display controller
		//LCD_PORT &= ~LCD_DC_PIN;
	    LCD_PORT |= LCD_CE_PIN;
	
		// Data/DC can be used as button inputs when not sending to LCD (/w pullups)
		LCD_DDR &= ~(LCD_DATA_PIN | LCD_DC_PIN);
		LCD_PORT |= LCD_DATA_PIN | LCD_DC_PIN;

		//----
	}


}
