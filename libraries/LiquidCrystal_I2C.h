#ifndef LIQUIDCRYSTAL_I2C_H
#define LIQUIDCRYSTAL_I2C_H

#include <avr/io.h>
#include <util/delay.h>

// LCD Ý2C adresi
#define LCD_ADDRESS 0x27

// LCD komutlarý
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// LCD sýnýfý
typedef struct {
	uint8_t address;
	uint8_t cols;
	uint8_t rows;
	uint8_t charsize;
	uint8_t backlight_val;
} LiquidCrystal_I2C;

void LCD_init(LiquidCrystal_I2C* lcd, uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows);
void LCD_begin(LiquidCrystal_I2C* lcd, uint8_t cols, uint8_t rows);
void LCD_clear(LiquidCrystal_I2C* lcd);
void LCD_home(LiquidCrystal_I2C* lcd);
void LCD_setCursor(LiquidCrystal_I2C* lcd, uint8_t col, uint8_t row);
void LCD_noDisplay(LiquidCrystal_I2C* lcd);
void LCD_display(LiquidCrystal_I2C* lcd);
void LCD_noCursor(LiquidCrystal_I2C* lcd);
void LCD_cursor(LiquidCrystal_I2C* lcd);
void LCD_noBlink(LiquidCrystal_I2C* lcd);
void LCD_blink(LiquidCrystal_I2C* lcd);
void LCD_scrollDisplayLeft(LiquidCrystal_I2C* lcd);
void LCD_scrollDisplayRight(LiquidCrystal_I2C* lcd);
void LCD_leftToRight(LiquidCrystal_I2C* lcd);
void LCD_rightToLeft(LiquidCrystal_I2C* lcd);
void LCD_autoscroll(LiquidCrystal_I2C* lcd);
void LCD_noAutoscroll(LiquidCrystal_I2C* lcd);
void LCD_createChar(LiquidCrystal_I2C* lcd, uint8_t, uint8_t[]);
void LCD_write(LiquidCrystal_I2C* lcd, uint8_t value);
void LCD_command(LiquidCrystal_I2C* lcd, uint8_t value);
void LCD_backlight(LiquidCrystal_I2C* lcd);
void LCD_noBacklight(LiquidCrystal_I2C* lcd);

#endif // LIQUIDCRYSTAL_I2C_H
