/******************************************************************************
 *
 * Copyright:
 *    (C) 2000 - 2005 Embedded Artists AB
 *
 *****************************************************************************/


#include "../pre_emptive_os/api/osapi.h"
#include "../pre_emptive_os/api/general.h"
#include <printf_P.h>
#include <lpc2xxx.h>
#include <consol.h>
#include <config.h>

/******************************************************************************
 * Defines and typedefs
 *****************************************************************************/
#define CRYSTAL_FREQUENCY FOSC
#define PLL_FACTOR        PLL_MUL
#define VPBDIV_FACTOR     PBSD

#define  SPI_CS   0x00008000  //<= new board, old board = 0x00800000



const tU8 eaText[][8] = {
		{0x10,0x30,0x70,0xff,0xff,0x70,0x30,0x10}, //down
		{0x18,0x18,0x18,0x18,0xff,0x7e,0x3c,0x18}, //right
		{0x08,0x0c,0x0e,0xff,0xff,0x0e,0x0c,0x08}, //up
		{0x18,0x3c,0x7e,0xff,0x18,0x18,0x18,0x18}, //left
		{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},  //blink
		{0xff,0x81,0x91,0xf1,0xff,0x81,0x91,0xf1},   //gg
		{0x03,0x03,0x03,0xff,0xff,0x03,0x03,0x03},  //T
		{0xff,0x91,0x91,0x6e,0xff,0x81,0x91,0xf1} //bg
		
		//and next...
	};

tU8 pattern[8];

void printMatrix(tU8 arrowKierunek)
{
	startTimer1(2);
	memcpy (pattern, eaText[arrowKierunek], 8);
	if(!(arrowKierunek == 4))
		osSleep(16);	
	else
		osSleep(32);
	return;
}
