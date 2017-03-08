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
#include "sounds/startup.h"
#include "sounds/error.h"
#include "sounds/win.h"

/******************************************************************************
 * Defines and typedefs
 *****************************************************************************/
#define CRYSTAL_FREQUENCY FOSC
#define PLL_FACTOR        PLL_MUL
#define VPBDIV_FACTOR     PBSD


void soundManager(int soundNumber)
{
	switch(soundNumber)
	{
		case 0: soundPlay(startupSound, startupSoundSize, 1200,soundNumber);
		break;
		case 1: soundPlay(errorSound, errorSoundSize, 1200,soundNumber);
		break;
		case 2: soundPlay(winSound, winSoundSize, 1200,soundNumber);
		break;
	}
}

void soundPlay(const char soundchar[],const int sizeofsc, int freq, int soundNumber)
{
    tU32 cnt = 0;
    tU32 i;
    IODIR |= 0x00000380; //buzzer na out
    IOCLR  = 0x00000380; //czyszczenie buzzera

    //
    //Initialize DAC: AOUT = P0.25
    //
    PINSEL1 &= ~0x000C0000; //ustawienie na wejście
    PINSEL1 |=  0x00080000; //ustawienie na wyjście
		cnt = 0;
		while(cnt++ < sizeofsc)
		{
			if(soundNumber==-1) break;
			tS32 val;

			val = soundchar[cnt] - 128;
			val = val * 2;
			if (val > 127) val = 127;
			else if (val < -127) val = -127;

			DACR = ((val+128) << 8) |  //actual value to output
			(1 << 16);         //BIAS = 1, 2.5uS settling time

			//delay 125 us = 850 for 8kHz, 600 for 11 kHz
			for(i=0; i<freq; i++)
				asm volatile (" nop"); //to robi dokladnie nic, zabiera jedno tickniecie procesora
		}
}
