#define F_CPU 1000000UL

#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.h"

// This cache is reseted per frame
unsigned char cpu_cache[128][8];

// This cache represents the LCD status on all times
unsigned char gpu_cache[128][8];


//HACK, only visible in the header file
void lcd_send(unsigned char data, LcdCmdData cd)
{
	// Data/DC are outputs for the lcd (all low)
	LCD_DDR |= LCD_DATA_PIN | LCD_DC_PIN;

	// Enable display controller (active low)
	LCD_PORT &= ~LCD_CE_PIN;

	// Either command or data
	if(cd == LCD_DATA) {
		LCD_PORT |= LCD_DC_PIN;
	} else {
		LCD_PORT &= ~LCD_DC_PIN;
	}

	char j;
	for(unsigned char i=0;i<8;i++) {

		// Set the DATA pin value
		if((data >> (7 - i)) & 0x01) {
			LCD_PORT |= LCD_DATA_PIN;
		} else {
			LCD_PORT &= ~LCD_DATA_PIN;
		}

		// Toggle the clock
		LCD_PORT |= LCD_CLK_PIN;
		for(j=0;j<4;j++); // lis?tty pient? viivett?
		LCD_PORT &= ~LCD_CLK_PIN;
	}

	// Disable display controller
	//LCD_PORT &= ~LCD_DC_PIN;
	LCD_PORT |= LCD_CE_PIN;

	// Data/DC can be used as button inputs when not sending to LCD (/w pullups)
	LCD_DDR &= ~(LCD_DATA_PIN | LCD_DC_PIN);
	LCD_PORT |= LCD_DATA_PIN | LCD_DC_PIN;
}


// currently the most minimal init with it has worked
// Contains some old code, could be cleaned up
void lcd_init(void)
{
	memset(cpu_cache, 0, sizeof cpu_cache);
	memset(gpu_cache, 0, sizeof gpu_cache);
/*
	// IO-port INIT
	DDRA = 0xE0;
	
	// Set output bits on lcd port
	LCD_DDR |= LCD_RST_PIN | LCD_CE_PIN | LCD_DC_PIN | LCD_DATA_PIN | LCD_CLK_PIN;

    // Disable LCD controller
    LCD_PORT |= LCD_CE_PIN;
*/


	// Pull-up on reset pin
    LCD_PORT |= LCD_RST_PIN;	// Reset = 1
	
	// Set output bits on lcd port
	LCD_DDR |= LCD_RST_PIN | LCD_CE_PIN | LCD_DC_PIN | LCD_DATA_PIN | LCD_CLK_PIN;
	// LCD_DDR DDRC ei toimimmm

	// Wait after VCC high for reset (max 30ms)
    _delay_ms(15);
    
    
    lcd_send(0xEB, LCD_CMD);  	// LCD bias 
    lcd_send(0x23, LCD_CMD);  	// Set Lines >> 23 = 64
    lcd_send(0x81, LCD_CMD);	// Set Potentiometer
    lcd_send(0x64, LCD_CMD);	// 16 >> 64 (Tummuus)
    lcd_send(0xAF, LCD_CMD);  	// Set Display ON
    lcd_send(0xCC, LCD_CMD);  	// Set LCD to RAM mapping
	
	//lcd_send(0xFF, LCD_DATA);	// 
	//lcd_send(0x00, LCD_DATA);  	// Set Display ON
    //lcd_send(0xFF, LCD_DATA);
    
    // Toggle display reset pin
    LCD_PORT &= ~LCD_RST_PIN; // Reset = 0
	_delay_ms(15);
    LCD_PORT |= LCD_RST_PIN;	// Reset = 1

	_delay_ms(15);

    // Disable LCD controller
    LCD_PORT |= LCD_CE_PIN;
}

// Set the base address of the lcd
// Hack version, only for internal use
void lcd_goto_xy_exact(unsigned char x, unsigned char y) {

	lcd_send(0xB0 | y, LCD_CMD);	// page

	lcd_send(0x00 | (x & 0x0F), LCD_CMD);
	lcd_send(0x10 | ((x & 0xF0)>>4), LCD_CMD);	// column
	
}

// hack clear
void lcd_clear()
{
	lcd_goto_xy_exact(0, 0);

	// Data/DC are outputs for the lcd (all low)
	LCD_DDR |= LCD_DATA_PIN | LCD_DC_PIN;
    // Enable display controller (active low)
    LCD_PORT &= ~LCD_CE_PIN;

	// DATA MODE ON 
	LCD_PORT |= LCD_DC_PIN;
	// COMMAND MODE 
	//LCD_PORT &= ~LCD_DC_PIN;

	//clear
	for(int q = 0; q < 3000; ++q)
	{
		for(unsigned char i=0;i<8;i++) {
			LCD_PORT &= ~LCD_DATA_PIN;

			// Toggle the clock
			LCD_PORT |= LCD_CLK_PIN;
			for(int j=0;j<10;j++); // lisätty pientä viivettä
			LCD_PORT &= ~LCD_CLK_PIN;
		}
	}

	//---- {Disabling controllers}

	// Disable display controller
	//LCD_PORT &= ~LCD_DC_PIN;
    LCD_PORT |= LCD_CE_PIN;

	// Data/DC can be used as button inputs when not sending to LCD (/w pullups)
	LCD_DDR &= ~(LCD_DATA_PIN | LCD_DC_PIN);
	LCD_PORT |= LCD_DATA_PIN | LCD_DC_PIN;
}


// Send data to the LCD driver, set mode before using
// LCD library internal usage only
void send_data(unsigned char data)
{
    int j;
    for(unsigned char i=0;i<8;i++)
     {
        // Set the DATA pin value
        if((data>>(7-i)) & 0x01)
        {
                LCD_PORT |= LCD_DATA_PIN;
        } else
        {
                LCD_PORT &= ~LCD_DATA_PIN;
        }
       
        // Toggle the clock
        LCD_PORT |= LCD_CLK_PIN;
        for(j=0;j<4;j++); // lisätty pientä viivettä
        LCD_PORT &= ~LCD_CLK_PIN;
    }

}


// Hack cursor position thing
void position_cursor(int x, int y)
{
/*
    //uusi koodi
    // Data/DC are outputs for the lcd (all low)
    LCD_DDR |= LCD_DATA_PIN | LCD_DC_PIN;
	// Enable display controller (active low)
	LCD_PORT &= ~LCD_CE_PIN;
*/

	    //CMD
	LCD_PORT &= ~LCD_DC_PIN;

	//Send Data
    send_data(0xB0 | y); //page
    send_data(0x00 | (x & 0x0F)); //column
    send_data(0x10 | ((x & 0xF0)>>4)); 

/*
	// Disable display controller
	LCD_PORT |= LCD_CE_PIN;

    // Data/DC can be used as button inputs when not sending to LCD (/w pullups)
    LCD_DDR &= ~(LCD_DATA_PIN | LCD_DC_PIN);
    LCD_PORT |= LCD_DATA_PIN | LCD_DC_PIN;
	*/
 
}

/* All rest of the code under this is good */

// Open control to the LCD display
void open_control()
{
	//--- {Enabling controllers}
	// Data/DC are outputs for the lcd (all low)
	LCD_DDR |= LCD_DATA_PIN | LCD_DC_PIN;
    // Enable display controller (active low)
	LCD_PORT &= ~LCD_CE_PIN;
}

// Close control to the LCD display
void close_control()
{
	
	//---- {Disabling controllers}

	// Disable display controller
	//LCD_PORT &= ~LCD_DC_PIN;
    LCD_PORT |= LCD_CE_PIN;

	// Data/DC can be used as button inputs when not sending to LCD (/w pullups)
	LCD_DDR &= ~(LCD_DATA_PIN | LCD_DC_PIN);
	LCD_PORT |= LCD_DATA_PIN | LCD_DC_PIN;

	//----
}

// Draw a single pixel
void lcd_pixel(int x, int y)
{
	// read state from CPU cache and write back to it

	//TODO: not tested
	const unsigned char cache = cpu_cache[x][y / 8];
	cpu_cache[x][y / 8] = 1 ? (1 << (y % 8)) | cache : ~(1 << (y % 8)) & cache;

}

// Flip the display and handle caches, should be called after drawing is done
// Steps:
// Compare cpu / gpu caches
// if difference
// 		-> Change cursor position
//      -> Send data over
//      -> Update GPU cache
//
// Clear CPU cache
void lcd_display()
{
	open_control();

	// lastX when draw happens, used to make less position_cursor calls
	int lastX = 0;

	for(int x = 0; x < 128; ++x)
	{
		for(int y = 0; y < 8; ++y)
		{
			if(cpu_cache[x][y] != gpu_cache[x][y])
			{
				// Update the cache
				gpu_cache[x][y] = cpu_cache[x][y];

				// Draws that are in sequence do not need cursor repositioning
				if(lastX == x - 1)
				{
					lastX++;
				}
				else
				{
					position_cursor(x, y);
				
					// DATA MODE ON, left off by the cursor position
					LCD_PORT |= LCD_DC_PIN;
				}

				send_data(gpu_cache[x][y]);
			}
		}
	}

	close_control();


	// Clear CPU cache
	memset(cpu_cache, 0, sizeof cpu_cache);
}
