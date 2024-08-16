#include "LiquidCrystal_I2C.h"

// Özel I2C fonksiyonlarý
void i2c_init() {
	// I2C initialization
	TWSR = 0x00;
	TWBR = 0x47;
	TWCR = (1 << TWEN);
}

void i2c_start() {
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
}

void i2c_stop() {
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
	while (TWCR & (1 << TWSTO));
}

void i2c_write(uint8_t data) {
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
}

void i2c_writeByte(uint8_t address, uint8_t data) {
	i2c_start();
	i2c_write(address << 1);
	i2c_write(data);
	i2c_stop();
}

// LCD fonksiyonlarý
void LCD_send(LiquidCrystal_I2C* lcd, uint8_t value, uint8_t mode) {
	uint8_t highnib = value & 0xf0;
	uint8_t lownib = (value << 4) & 0xf0;
	i2c_writeByte(lcd->address, highnib | mode | lcd->backlight_val);
	i2c_writeByte(lcd->address, highnib | mode | lcd->backlight_val | 0x04);
	i2c_writeByte(lcd->address, highnib | mode | lcd->backlight_val);
	i2c_writeByte(lcd->address, lownib | mode | lcd->backlight_val);
	i2c_writeByte(lcd->address, lownib | mode | lcd->backlight_val | 0x04);
	i2c_writeByte(lcd->address, lownib | mode | lcd->backlight_val);
}

void LCD_command(LiquidCrystal_I2C* lcd, uint8_t value) {
	LCD_send(lcd, value, 0);
}

void LCD_write(LiquidCrystal_I2C* lcd, uint8_t value) {
	LCD_send(lcd, value, 0x01);
}

void LCD_init(LiquidCrystal_I2C* lcd, uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows) {
	lcd->address = lcd_addr;
	lcd->cols = lcd_cols;
	lcd->rows = lcd_rows;
	lcd->charsize = LCD_5x8DOTS;
	lcd->backlight_val = 0x08;  // Arka ýþýðý aç
	i2c_init();
	_delay_ms(50);
	LCD_command(lcd, LCD_FUNCTIONSET | LCD_4BITMODE | LCD_2LINE | lcd->charsize);
	LCD_command(lcd, LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF);
	LCD_command(lcd, LCD_CLEARDISPLAY);
	LCD_command(lcd, LCD_ENTRYMODESET | LCD_ENTRYLEFT);
	LCD_backlight(lcd); // Arka ýþýðý aç
}

void LCD_clear(LiquidCrystal_I2C* lcd) {
	LCD_command(lcd, LCD_CLEARDISPLAY);
	_delay_ms(2);
}

void LCD_home(LiquidCrystal_I2C* lcd) {
	LCD_command(lcd, LCD_RETURNHOME);
	_delay_ms(2);
}

void LCD_setCursor(LiquidCrystal_I2C* lcd, uint8_t col, uint8_t row) {
	uint8_t row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	if (row > lcd->rows) {
		row = lcd->rows - 1;
	}
	LCD_command(lcd, LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void LCD_backlight(LiquidCrystal_I2C* lcd) {
	lcd->backlight_val = 0x08;
	i2c_writeByte(lcd->address, lcd->backlight_val);
}

void LCD_noBacklight(LiquidCrystal_I2C* lcd) {
	lcd->backlight_val = 0x00;
	i2c_writeByte(lcd->address, lcd->backlight_val);
}
