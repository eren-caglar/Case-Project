#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "LiquidCrystal_I2C.h"
#include <stdio.h>
#include <stdbool.h>

#define DEBOUNCE_DELAY 300 // 50ms debounce delay

volatile uint32_t timer_millis = 0;
volatile uint32_t last_interrupt_time = 0; // Tekrardan tanýmlamayý engelledik
volatile char key;
volatile char inputBuffer[6] = {'\0'};
volatile uint8_t bufferIndex = 0;
volatile uint8_t inputComplete = 0;
volatile char pressed_key;
const char password[6]= {'5','1','9','5','1','9'};
volatile char entered_pass[6];
volatile int index_of_pass=0;

LiquidCrystal_I2C lcd;

char keypad[4][4] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'}
};

// Fonksiyon Prototipleri
void startingScreen(LiquidCrystal_I2C lcd);
void dispNum(char key);
void passScreen(LiquidCrystal_I2C lcd);
bool passCheck(const char password[], volatile char entered_pass[], int boyut);
void correctPass(LiquidCrystal_I2C lcd);
void wrongPass(LiquidCrystal_I2C lcd);
void init_timer0();
uint32_t millis();
void servo_init();
void servo_set_position(uint16_t angle);
void servo_stop();
void lockingScreen(LiquidCrystal_I2C lcd);
void lockedScreen(LiquidCrystal_I2C lcd);

// Timer0 Overflow ISR - every 1ms
ISR(TIMER0_COMPA_vect) {
	timer_millis++;
}

uint32_t millis() {
	uint32_t millis_return;

	cli(); // Kesme sýrasýnda deðiþkeni okuyabilmek için kesmeyi geçici olarak kapat
	millis_return = timer_millis;
	sei(); // Kesme iznini tekrar aktif et

	return millis_return;
}

void init_timer0() {
	TCCR0A = (1 << WGM01); // CTC modunu seç
	OCR0A = 249;           // 1ms için compare deðerini ayarla (16MHz clock, 64 prescaler)
	TIMSK0 = (1 << OCIE0A); // Timer compare kesmesini etkinleþtir
	TCCR0B = (1 << CS01) | (1 << CS00); // Prescaler olarak 64 seç
}

ISR(PCINT1_vect){ // When any PCINT3:0 change
	uint32_t current_time = millis(); // Zamanlayýcýyý okuyun
	if (current_time - last_interrupt_time > DEBOUNCE_DELAY) { // Debounce süresini kontrol edin
		last_interrupt_time = current_time;

		PORTD = ~(0b00000100); // 1st row low
		_delay_ms(1); // Wait for port delay
		if(~PINC & (1<<PINC0)){key='1';} // 1
		else if(~PINC & (1<<PINC1)){key='2';} // 2
		else if(~PINC & (1<<PINC2)){key='3';} // 3
		else if(~PINC & (1<<PINC3)){key='A';} // A
		
		PORTD = ~(0b00001000); // 2nd row low
		_delay_ms(1); // Wait for port delay
		if(~PINC & (1<<PINC0)){key='4';} // 4
		else if(~PINC & (1<<PINC1)){key='5';} // 5
		else if(~PINC & (1<<PINC2)){key='6';} // 6
		else if(~PINC & (1<<PINC3)){key='B';} // B
		
		PORTD = ~(0b00010000); // 3rd row low
		_delay_ms(1); // Wait for port delay
		if(~PINC & (1<<PINC0)){key='7';} // 7
		else if(~PINC & (1<<PINC1)){key='8';} // 8
		else if(~PINC & (1<<PINC2)){key='9';} // 9
		else if(~PINC & (1<<PINC3)){key='C';} // C
		
		PORTD = ~(0b00100000); // 4th row low
		_delay_ms(1); // Wait for port delay
		if(~PINC & (1<<PINC0)){key='*';} // *
		else if(~PINC & (1<<PINC1)){key='0';} // 0
		else if(~PINC & (1<<PINC2)){key='#';} // #
		else if(~PINC & (1<<PINC3)){key='D';} // D
		
		PORTD = 0x00; // All rows low
		
		if(pressed_key == '*'){
			if(index_of_pass < 6) { // Þifre boyutunu kontrol et
				dispNum(key);
				entered_pass[index_of_pass] = key;
				index_of_pass++;
			}
		}
	}
}

int main(void) {
	int pressed_key_flag = 0;
	// Keypad konfigürasyonlarý
	DDRD = 0b00111100; // PD5:2 output
	PORTD = 0x00; // PD5:2 low
	DDRC = 0x00; // PB3:0 input
	PORTC = 0x0F; // PB3:0 pull-up enable
	
	// Pin Change Interrupt Settings
	PCICR |= (1<<PCIE1); // PCINT PortB enable
	PCMSK1 = 0x0F; // PCINT3:0 enable
	sei();
	
	init_timer0(); // Timer0'ý baþlat
	sei();         // Global kesmeleri etkinleþtir
	
	startingScreen(lcd);
	
	while (1) {
		if(key == '*'){
			pressed_key = '*';
			passScreen(lcd);
			while(pressed_key == '*') {
				if(index_of_pass == 6){
					if(passCheck(password, entered_pass, 3)) {
						int screenFlag = 0;
						if(screenFlag == 0){
						correctPass(lcd);
						screenFlag = 1;
						}
						// kasayi ac motoru dondur
							servo_init();
							//servo_set_position(90);  // Servo motoru 90 derece döndür
							//_delay_ms(1000);  // 2 saniye bekle
							servo_set_position(180);  // Servo motoru 90 derece döndür
							_delay_ms(220);  // 2 saniye bekle
							servo_stop();  // Servo motoru durdur
						   lockingScreen(lcd);
							while(1){
								if(key == '*'){ // kasayi tekrar kilitle
									servo_init();
									//servo_set_position(90);  // Servo motoru 90 derece döndür
									//_delay_ms(1000);  // 2 saniye bekle
									servo_set_position(180);  // Servo motoru 90 derece döndür
									_delay_ms(220);  // 2 saniye bekle
									servo_stop();  // Servo motoru durdur
									lockedScreen(lcd);
									_delay_ms(4000);
									break;
								}
							}
							key = 'x';
							pressed_key = 'x';
							index_of_pass = 0;
							pressed_key_flag = 1;
							break;
						} else {
						wrongPass(lcd); // yanlis sifre desin, entered_pass'ý sifirlasýn ve ana ekraný cagirip donguden ciksin
						_delay_ms(4000);
						index_of_pass = 0;
						pressed_key_flag = 1;
						key = 'x';
						pressed_key = 'x';
						break;
					}
					
				}
			}
		}
		
		if(pressed_key_flag == 1){
			startingScreen(lcd);
			pressed_key_flag = 0;
		}
		
		
		
		
		
	}
}

void startingScreen(LiquidCrystal_I2C lcd){
	LCD_init(&lcd, 0x27, 16, 2);
	LCD_clear(&lcd);
	LCD_backlight(&lcd);
	LCD_setCursor(&lcd, 0, 0);
	LCD_write(&lcd, 'M');
	LCD_write(&lcd, 'e');
	LCD_write(&lcd, 'r');
	LCD_write(&lcd, 'h');
	LCD_write(&lcd, 'a');
	LCD_write(&lcd, 'b');
	LCD_write(&lcd, 'a');
	LCD_write(&lcd, '!');
	LCD_setCursor(&lcd, 0, 1);
	LCD_write(&lcd, 'L');
	LCD_write(&lcd, 'u');
	LCD_write(&lcd, 't');
	LCD_write(&lcd, 'f');
	LCD_write(&lcd, 'e');
	LCD_write(&lcd, 'n');
	LCD_write(&lcd, ' ');
	LCD_write(&lcd, '*');
	LCD_write(&lcd, '\'');
	LCD_write(&lcd, 'a');
	LCD_write(&lcd, ' ');
	LCD_write(&lcd, 'b');
	LCD_write(&lcd, 'a');
	LCD_write(&lcd, 's');
	LCD_write(&lcd, 'i');
	LCD_write(&lcd, 'n');
}

void passScreen(LiquidCrystal_I2C lcd){
	LCD_init(&lcd, 0x27, 16, 2);
	LCD_clear(&lcd);
	LCD_backlight(&lcd);
	LCD_setCursor(&lcd, 0, 0);
	LCD_write(&lcd, 'L');
	LCD_write(&lcd, 'u');
	LCD_write(&lcd, 't');
	LCD_write(&lcd, 'f');
	LCD_write(&lcd, 'e');
	LCD_write(&lcd, 'n');
	LCD_write(&lcd, ' ');
	LCD_write(&lcd, 's');
	LCD_write(&lcd, 'i');
	LCD_write(&lcd, 'f');
	LCD_write(&lcd, 'r');
	LCD_write(&lcd, 'e');
	LCD_write(&lcd, 'y');
	LCD_write(&lcd, 'i');
	LCD_setCursor(&lcd, 0, 1);
	LCD_write(&lcd, 'g');
	LCD_write(&lcd, 'i');
	LCD_write(&lcd, 'r');
	LCD_write(&lcd, 'i');
	LCD_write(&lcd, 'n');
	LCD_write(&lcd, 'i');
	LCD_write(&lcd, 'z');
	LCD_write(&lcd, '.');
}

bool passCheck(const char password[], volatile char entered_pass[], int boyut) {
	for(int i = 0; i < boyut; i++) {
		if(password[i] != entered_pass[i]) {
			return false;
		}
	}
	return true;
}

void correctPass(LiquidCrystal_I2C lcd){
	LCD_init(&lcd, 0x27, 16, 2);
	LCD_clear(&lcd);
	LCD_backlight(&lcd);
	LCD_setCursor(&lcd, 0, 0);
	LCD_write(&lcd, 'S');
	LCD_write(&lcd, 'i');
	LCD_write(&lcd, 'f');
	LCD_write(&lcd, 'r');
	LCD_write(&lcd, 'e');
	LCD_write(&lcd, ' ');
	LCD_write(&lcd, 'd');
	LCD_write(&lcd, 'o');
	LCD_write(&lcd, 'g');
	LCD_write(&lcd, 'r');
	LCD_write(&lcd, 'u');
	LCD_write(&lcd, '!');
	LCD_setCursor(&lcd, 0, 1);
	LCD_write(&lcd, 'K');
	LCD_write(&lcd, 'a');
	LCD_write(&lcd, 's');
	LCD_write(&lcd, 'a');
	LCD_write(&lcd, ' ');
	LCD_write(&lcd, 'a');
	LCD_write(&lcd, 'c');
	LCD_write(&lcd, 'i');
	LCD_write(&lcd, 'l');
	LCD_write(&lcd, 'i');
	LCD_write(&lcd, 'y');
	LCD_write(&lcd, 'o');
	LCD_write(&lcd, 'r');
	LCD_write(&lcd, '!');
}

void wrongPass(LiquidCrystal_I2C lcd){
	LCD_init(&lcd, 0x27, 16, 2);
	LCD_clear(&lcd);
	LCD_backlight(&lcd);
	LCD_setCursor(&lcd, 0, 0);
	LCD_write(&lcd, 'S');
	LCD_write(&lcd, 'i');
	LCD_write(&lcd, 'f');
	LCD_write(&lcd, 'r');
	LCD_write(&lcd, 'e');
	LCD_write(&lcd, ' ');
	LCD_write(&lcd, 'y');
	LCD_write(&lcd, 'a');
	LCD_write(&lcd, 'n');
	LCD_write(&lcd, 'l');
	LCD_write(&lcd, 'i');
	LCD_write(&lcd, 's');
	LCD_write(&lcd, '!');
}


void lockingScreen(LiquidCrystal_I2C lcd){
	LCD_init(&lcd, 0x27, 16, 2);
	LCD_clear(&lcd);
	LCD_backlight(&lcd);
	LCD_setCursor(&lcd, 0, 0);
	LCD_write(&lcd, 'K');
	LCD_write(&lcd, 'i');
	LCD_write(&lcd, 'l');
	LCD_write(&lcd, 'i');
	LCD_write(&lcd, 't');
	LCD_write(&lcd, 'l');
	LCD_write(&lcd, 'e');
	LCD_write(&lcd, 'm');
	LCD_write(&lcd, 'e');
	LCD_write(&lcd, 'k');
	LCD_write(&lcd, ' ');
	LCD_write(&lcd, 'i');
	LCD_write(&lcd, 'c');
	LCD_write(&lcd, 'i');
	LCD_write(&lcd, 'n');
	LCD_setCursor(&lcd, 0, 1);
	LCD_write(&lcd, '*');
	LCD_write(&lcd, '\'');
	LCD_write(&lcd, 'a');
	LCD_write(&lcd, ' ');
	LCD_write(&lcd, 'b');
	LCD_write(&lcd, 'a');
	LCD_write(&lcd, 's');
	LCD_write(&lcd, 'i');
	LCD_write(&lcd, 'n');

}

void lockedScreen(LiquidCrystal_I2C lcd){
	LCD_init(&lcd, 0x27, 16, 2);
	LCD_clear(&lcd);
	LCD_backlight(&lcd);
	LCD_setCursor(&lcd, 0, 0);
	LCD_write(&lcd, 'K');
	LCD_write(&lcd, 'a');
	LCD_write(&lcd, 's');
	LCD_write(&lcd, 'a');
	LCD_setCursor(&lcd, 0, 1);
	LCD_write(&lcd, 'k');
	LCD_write(&lcd, 'i');
	LCD_write(&lcd, 'l');
	LCD_write(&lcd, 'i');
	LCD_write(&lcd, 't');
	LCD_write(&lcd, 'l');
	LCD_write(&lcd, 'e');
	LCD_write(&lcd, 'n');
	LCD_write(&lcd, 'd');
	LCD_write(&lcd, 'i');
	
	
	
}

void dispNum(char key){
	LCD_init(&lcd, 0x27, 16, 2);
	LCD_clear(&lcd);
	LCD_backlight(&lcd);
	LCD_setCursor(&lcd, 0, 0);
	LCD_write(&lcd, key);
}

void servo_init() {
	// Timer1'i hýzlý PWM modu, 16-bit prescaler ile baþlatýn
	TCCR1A = (1 << WGM11) | (1 << COM1A1);  // Fast PWM, ICR1 üst sýnýr, Clear OC1A on compare match
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);  // Prescaler 8, Fast PWM, ICR1 kullan
	ICR1 = 19999;  // 20ms periyod için ICR1 üst sýnýrý (16MHz / 8 prescaler = 2MHz, 2MHz / 50Hz = 20000 counts per 20ms)
	DDRB |= (1 << PB1);  // PB1 (pin 9) çýkýþ olarak ayarlanýr
}

void servo_set_position(uint16_t angle) {
	// 0 derece için 1000, 90 derece için 1500, 180 derece için 2000 mikro saniye
	uint16_t pulse_width = (1000 + ((angle * 1500) / 180));  // Açýyý 1000-2000 mikro saniyeye dönüþtür
	OCR1A = (pulse_width);  // OCR1A'ya atanan deðer mikro saniyeler cinsinden hesaplanýr
}

void servo_stop() {
	TCCR1A = 0;  // Timer1'i durdur
	TCCR1B = 0;
}

