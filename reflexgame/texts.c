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

void printIntro(void)
{
	printf("\n\n\n\n\n*******************************************************\n");
	printf("* Welcome into LPCReflex, the Game                    *\n");
	printf("*                                                     *\n");
	printf("* This is the modfied test program                    *\n");
	printf("* for Embedded Artists' by Dawid Michalowski,         *\n");
	printf("* Karolina Dabrowska and Katarzyna Meckier.           *\n");
	printf("* LPC2148 Education Board v3.0...                     *\n");
 	printf("*                                                     *\n");
	printf("* Version: 3.0                                        *\n");
	printf("* Date:    28 December 2016                           *\n");
	printf("* (C) Embedded Artists 2005-2007                      *\n");
	printf("*                                                     *\n");
	printf("*******************************************************\n");
}

void printInfo(void)
{
	printf("\n*******************************************************\n");
	printf("* REFLEX INSTRUCTION                                  *\n");
	printf("*******************************************************\n");
	printf("* 1.Follow the screen and press correct buttons.      *\n");
	printf("* 2.You got 5s to react, otherwise you automatically  *\n");
	printf("*   lose!                                             *\n");
	printf("* 3.After played game you will see scoreboard here.   *\n");
	printf("* 4.Have fun!                                         *\n");
	printf("*******************************************************\n");
}

void printPatternInfo(void)
{
	printf("\n*******************************************************\n");
	printf("* PATTERN INFO                                        *\n");
	printf("*******************************************************\n");
	printf("* 0 - DOWN                                            *\n");
	printf("* 1 - RIGHT                                           *\n");
	printf("* 2 - UP                                              *\n");
	printf("* 3 - LEFT                                            *\n");
	printf("* 4 - BLINK                                           *\n");
	printf("* 5 - GG                                              *\n");
	printf("* 6 - T                                               *\n");
	printf("* 7 - BG                                              *\n");    
	printf("*******************************************************\n");
}

void printScoreboard(char table[10][128], int sc)
{
	
	int sizeOfRecords;
	int i;
	if(sc>10)
		sizeOfRecords=10;
	else
		sizeOfRecords=sc;
	printf("\n*******************************************************\n");
	printf("*            SCOREBOARD FOR LAST 10 GAMES             *\n");
	printf("*******************************************************\n");
	for(i=0; i<sizeOfRecords; i++)
		printf(table[i]);
	printf("*******************************************************\n");
}
