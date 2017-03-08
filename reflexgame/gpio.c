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


#define USEDBUTTONNUMBER 5

tU8 iopinTemp[USEDBUTTONNUMBER]={0};

tU8 checkIO(void)
{
	int i;
	tU8 curriopin[USEDBUTTONNUMBER];
	int j[USEDBUTTONNUMBER]={14,17,18,19,20}; //IOPIN bit button number
	
	//readiopins and put in array
	for(i=0; i<USEDBUTTONNUMBER; i++)
	{
		if(!(IOPIN0 & (1<<j[i])))
			curriopin[i]=1;
		else
			curriopin[i]=0;
	}
	
	//check iopin equality
	if(!(memcmp(curriopin, iopinTemp, sizeof(curriopin))))
		return 0;
	else
	{
		memcpy(iopinTemp,curriopin,5);
		return 1;
	}
}

int getButton(void)
{
	IODIR0 &= ~((1<<18));
	IODIR0 &= ~((1<<19));
	IODIR0 &= ~((1<<20));
	IODIR0 &= ~((1<<17));
	if(!(IOPIN0 & (1<<17))) //joy up
	{
		//IOSET0 |= (1<<17);
		return 2;
	}
	if(!(IOPIN0 & (1<<20))) //joy down
	{
		//IOSET0 |= (1<<20);
		return 0;
	}
	if(!(IOPIN0 & (1<<19))) //joy left
	{
		//IOSET0 |= (1<<19);
		return 3;
	}
	if(!(IOPIN0 & (1<<18))) //joy right
	{
		//IOSET0 |= (1<<18);
		return 1;
	}
	
	return -1;
	
}

int getAnyButton()
{
	//SET ON INPUT p0.*
	IODIR0 &= ~((1<<14));
	IODIR0 &= ~((1<<18));
	IODIR0 &= ~((1<<19));
	IODIR0 &= ~((1<<20));
	IODIR0 &= ~((1<<17));
	
	if(checkIO()) //ANITHOLDER
		if ((!(IOPIN0 & (1<<14))) || (!(IOPIN0 & (1<<17))) || (!(IOPIN0 & (1<<20))) || (!(IOPIN0 & (1<<19))) || (!(IOPIN0 & (1<<18)))) //if any btn press
		{
			return 1;
		}	
		else
			return 0;
	else
		return 0;
}

