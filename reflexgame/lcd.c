#include "../pre_emptive_os/api/osapi.h"
#include "../pre_emptive_os/api/general.h"
#include <printf_P.h>
#include <lpc2xxx.h>
#include <consol.h>

#define LCD_DATA      0x00ff0000  //P1.16-P1.23
#define LCD_RS        0x01000000  //P1.24
#define LCD_E         0x02000000  //P1.25
#define LCD_RW        0x00400000  //P0.22
#define LCD_BACKLIGHT 0x40000000  //P0.30

static void initLCD(void)
{
	IODIR1 |= (LCD_DATA | LCD_E | LCD_RS);
	IOCLR1  = (LCD_DATA | LCD_E | LCD_RS);

	IODIR0 |= LCD_RW;
	IOCLR0  = LCD_RW;

	IODIR0 |= LCD_BACKLIGHT;
	IOCLR0  = LCD_BACKLIGHT;
}

static void delay37us(void)
{
	volatile tU32 i; // Volatile keyword indicates that a value may change between different accesses,
	//even if it does not appear to be modified.

	//Temp test for 140 uS delay
	for(i=0; i<6*2500; i++)
		asm volatile (" nop"); //delay 15 ns x 2500 = about 37 us delay
}

static void writeLCD(tU8 reg, tU8 data)
{
	volatile tU8 i;

	if (reg == 0)
		IOCLR1 = LCD_RS;
	else
		IOSET1 = LCD_RS;

	IOCLR0 = LCD_RW;
	IOCLR1 = LCD_DATA;
	IOSET1 = ((tU32)data << 16) & LCD_DATA;

	IOSET1 = LCD_E;
	//	osSleep(1);
	for(i=0; i<16; i++)
		asm volatile (" nop"); //delay 15 ns x 16 = about 250 ns delay
	IOCLR1 = LCD_E;
	//	osSleep(1);
	for(i=0; i<16; i++)
		asm volatile (" nop"); //delay 15 ns x 16 = about 250 ns delay
}

static void lcdBacklight(tU8 onOff)
{
	if (onOff == TRUE)
		IOSET0 = LCD_BACKLIGHT;
	else
		IOCLR0 = LCD_BACKLIGHT;
}

// Wrties the strign of characters as a message on the LCD screen.
void messageOnLCD(char *str, tU8 important)
{
	static tU8 initialized;

	if(initialized == 0) 
	{
		initLCD();
		initialized = 1;
	}

	lcdBacklight(TRUE);
	osSleep(50);

	//function set
	writeLCD(0, 0x30);
	osSleep(1);
	writeLCD(0, 0x30);
	delay37us();
	writeLCD(0, 0x30);
	delay37us();

	//function set
	writeLCD(0, 0x38);
	delay37us();

	//display off
	writeLCD(0, 0x08);
	delay37us();

	//display clear
	writeLCD(0, 0x01);
	osSleep(1); //actually only 1.52 mS needed

	//display control set
	writeLCD(0, 0x06);
	osSleep(1);

	//display control set
	writeLCD(0, 0x0c);
	delay37us();

	//cursor home
	writeLCD(0, 0x02);
	osSleep(1);

	for(str; *str; str++)
	{
		if(*str != '\n')
		{
			writeLCD(1, *str);
		}
		else
		{
			writeLCD(0, 0x80 | 0x40);
		}
		delay37us();
	}
	delay37us();
	lcdBacklight(important);
}
