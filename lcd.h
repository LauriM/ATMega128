/*********************************************************
** lcd.h
**
** LCD driver
** Target: ATMEGA :: AVR-GCC
***********************************************************/
#ifndef TRUE_LCD_H
#define TRUE_LCD_H

/* Pinout for LCD */
#define LCD_CLK_PIN 	(1<<PC4)
#define LCD_DATA_PIN 	(1<<PC3)
#define LCD_DC_PIN 		(1<<PC2)
#define LCD_CE_PIN 		(1<<PC1)
#define LCD_RST_PIN 	(1<<PC0)
#define LCD_PORT		PORTC
#define LCD_DDR			DDRC

/* Special Chars */
#define ARROW_RIGHT	ICON(0)
#define ARROW_LEFT 	ICON(1)
#define ARROW_UP 	ICON(2)
#define ARROW_DOWN 	ICON(3)
#define STOP 		ICON(4)
#define PLAY	 	ICON(5)

void lcd_init(void);

/* Command type sent to the lcd */
typedef enum { LCD_CMD  = 0, LCD_DATA = 1 } LcdCmdData;

//TODO: Still to refactor these!
void lcd_goto_xy_exact(unsigned char x, unsigned char y);
void lcd_clear();

// *****
// These are the functions that should be exposed to the user

// Draw a single pixel
void lcd_pixel(int x, int y);

// Flip the display and handle caches, should be called after drawing is done
void lcd_display();

#endif



