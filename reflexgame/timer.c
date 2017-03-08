//timer.c
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

//fosc = 12mhz
//ppl_mul = 5
//pbsd = 1

#define  SPI_CS   0x00008000  //<= new board, old board = 0x00800000
void ledMatrix(void);
int getAnyButton();
tU32 timer=0;

void startTimer1(void)
{
  //initialize VIC for Timer1 interrupts
	VICIntSelect &= ~0x20;           //Timer1 interrupt is assigned to IRQ (not FIQ)
	VICVectAddr5  = (tU32)ledMatrix; //register ISR address
	VICVectCntl5  = 0x25;            //enable vector interrupt for timer1
	VICIntEnable  = 0x20;            //enable timer1 interrupt
  
  //initialize and start Timer #0
	T1TCR = 0x00000002;                           //disable and reset Timer1
	T1PR  = 0x0; 							//PR- WARTOSC DO KTOREJ DAZY PC, jedno przeskoczenie bitowe trwa 1/60 000 000 poniewaz proc ma 60Mhz
	T1PC  = 0x00000000;                           //PC- rozmiar oczekiwania pomiedzy skokami TC,
	T1MR0 = 2 *                           //calculate no of timer ticks
         ((CRYSTAL_FREQUENCY * PLL_FACTOR) / (1000 * VPBDIV_FACTOR));
	T1IR  = 0x000000ff;                           //reset all flags before enable IRQs
	T1MCR = 0x00000003;                          //reset counter and generate IRQ on MR0 match
	T1TCR = 0x00000001;                           //start Timer1	
}

int getSomeTime(void)
{
	timer=0; //timer nullfier
	for(;;)
	{
		if (getAnyButton())//tu klik cokolwiek
		{
			return timer; //return time
		}
		osSleep(1);
	}
}
