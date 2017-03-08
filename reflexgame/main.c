/******************************************************************************
 *
 * Copyright:
 *    (C) 2000 - 2007 Embedded Artists AB
 *
 * Description:
 *		Reflex, The Game
 *
 * 		Basically it's modified Embedded Artists' test program by Dawid Michalowski, Karolina Dabrowska 
 *		and Katarzyna Meckier.
 *	  
 *
 *****************************************************************************/

#include "../pre_emptive_os/api/osapi.h"
#include "../pre_emptive_os/api/general.h"
#include <printf_P.h>
#include <ea_init.h>
#include <lpc2xxx.h>
#include <consol.h>


#define GAMEPROC_STACK_SIZE 2048
#define MOTORPROC_STACK_SIZE 2048
#define MATRIXPROC_STACK_SIZE 2048
#define SOUNDPROC_STACK_SIZE 2048

#define INIT_STACK_SIZE  400
#define NUMBER_OF_ARROWS 10
#define  SPI_CS   0x00008000


/***************   STACKs   *****************/
static tU8 gameProcStack[GAMEPROC_STACK_SIZE];
static tU8 motorProcStack[MOTORPROC_STACK_SIZE];
static tU8 matrixProcStack[MATRIXPROC_STACK_SIZE];
static tU8 soundProcStack[SOUNDPROC_STACK_SIZE];
static tU8 initStack[INIT_STACK_SIZE];

/***************   PIDs   ******************/
static tU8 pid1;
static tU8 pid2;
static tU8 pid3;
static tU8 pid4;

/**************   PROCs   ******************/
static void gameProc(void* arg);
static void motorProc(void* arg);
static void matrixProc(void* arg);
static void soundProc(void* arg);
static void initProc(void* arg);

/**************  FUNCTIONs   ***************/
void printMatrix(tU8 arrowKierunek);
void startTimer1(void);
int getAnyButton(void);
int getButton(void);
tU8 checkIO(void);
int getSomeTime(void);
void soundManager(int soundNumber);
void messageOnLCD(char *str, tU8 important);
void testRGB(tU8 r, tU8 g, tU8 b);
void messageOnLCD(char *str, tU8 important);
void testMotor(void);
void printIntro(void);
void printInfo(void);
void printPatternInfo(void);
void printScoreboard(char table[10][128], int sc);

/************   GLOBAL VARs   **************/
int soundNumber=-1;
int ledPattern=4;
volatile tU32 msClock = 0;
tU8 motorplay=0;

int main(void)
{
	tU8 error;
	tU8 pid;

	//immediately turn off buzzer (if connected)
	IODIR0 |= 0x00000080;
	IOSET0  = 0x00000080;

	osInit();
	osCreateProcess(initProc, initStack, INIT_STACK_SIZE, &pid, 1, NULL, &error);
	osStartProcess(pid, &error);

	osStart();
	return 0;
}

static void initProc(void* arg)
{
	tU8 error;

	eaInit();
	osCreateProcess(gameProc, gameProcStack, GAMEPROC_STACK_SIZE, &pid1, 3, NULL, &error);
	osStartProcess(pid1, &error);

	osDeleteProcess();
}

void appTick(tU32 elapsedTime)
{
	msClock += elapsedTime;
}

static void gameProc(void* arg)
{
	testRGB(255, 255, 255);
	tU8  error;
	printIntro();
	osCreateProcess(matrixProc, matrixProcStack, MATRIXPROC_STACK_SIZE, &pid2, 3, NULL, &error);
	printf("Launching LED Matrix...\n");
	messageOnLCD("LED Matrix\nruns...", 1);
	osStartProcess(pid2, &error);
	printPatternInfo();
	osCreateProcess(motorProc, motorProcStack, MOTORPROC_STACK_SIZE, &pid3, 3, NULL, &error);
	printf("Motor test...\n");
	messageOnLCD("Motor\ntest...", 1);
	osStartProcess(pid3, &error);
	messageOnLCD("Reflex, The Game\nLoading...", 1);
	printf("Playing startup sound...\n");
	soundManager(0); //Playing startup without proc
	osCreateProcess(soundProc, soundProcStack, SOUNDPROC_STACK_SIZE, &pid4, 3, NULL, &error); 
	printf("Launching sound manager process...\n");
	osStartProcess(pid4, &error);
	messageOnLCD(" For more info\n check terminal.", 1);
	printInfo();
	osSleep(64);  //wait for a short
	
	//main game code here
	int j;
	int timeInMs, avgTime;
	int sumTime=0;
	tU32 seed;
	char scoreBoard[10][128];
	tU8 scorecounter=0;
	int line;
	char buffer[32];
	for(;;)
	{
		printf("Game loop starts...\n");
		sumTime=0;
		ledPattern=6;
		printf("LEDMATRIX: blinking %d pattern...\n", ledPattern);
		testRGB(0, 0, 255);
		printf("RGB: emitting blue...\n");
		messageOnLCD("Reflex, The Game\nPRESS ANY BUTTON", 1);
		printf("Getting seed...\n");
		seed=getSomeTime();
		printf("Seed: %d\n", seed);
		srand(seed);
		printf("Planted a seed in srand-ground!\n");
		ledPattern=4;
		printf("LEDMATRIX: blinking %d pattern...\n", ledPattern);
		osSleep(32);
		messageOnLCD(" Let the hunger \n  games begin!  ", 1);
		for(j=0;j<NUMBER_OF_ARROWS;j++)
		{	
			ledPattern=rand() % 4;
			printf("LEDMATRIX: blinking %d pattern...\n", ledPattern);
			motorplay=1;
			//osSleep(2); //lil helper
			timeInMs = getSomeTime();
			motorplay=0;
			sumTime += timeInMs;
			printf("Time: %dms\n", timeInMs);
			if(getButton()!=ledPattern)
				break;
			if(timeInMs>5000)
			{
				messageOnLCD("Elapsed time:\nmore than 5s!", 1);
				printf("Time crossed a deadline (5s)!\n");
				osSleep(100);
				break;
			}
			ledPattern=4;
			printf("LEDMATRIX: blinking %d pattern...\n", ledPattern);
			snprintf(buffer, sizeof(buffer), "Elapsed time:\n%dms", timeInMs);
			messageOnLCD(buffer,1);
			osSleep(32);
		}
		if(j==NUMBER_OF_ARROWS)
		{
			ledPattern=5;
			printf("LEDMATRIX: blinking %d pattern...\n", ledPattern);
			testRGB(0, 255, 0);
			printf("RGB: emitting green...\n");
			avgTime=sumTime/NUMBER_OF_ARROWS;
			messageOnLCD("   Good game!   \n    You won!    ", 1);
			soundNumber = 2;
			printf("Playing win sound...\n");
			osSleep(300);
			snprintf(buffer, sizeof(buffer), "Avg time:\n%dms", avgTime);
			messageOnLCD(buffer,1);
			osSleep(64);
			messageOnLCD("   Good game!   \n    You won!    ", 1);
			printf("Good game! Avg time in this game: %dms\n", avgTime);
			scorecounter++;
			line = (scorecounter-1)%10;
			printf("Filling scoreboard...\n", ledPattern);
			snprintf(scoreBoard[line], 128, "PLAYER %d | Avg. time: %dms\n", scorecounter, avgTime);
		}
		else
		{
			ledPattern=7; //tutaj pattern bg
			printf("LEDMATRIX: blinking %d pattern...\n", ledPattern);
			testRGB(255, 0, 0); //led bg
			printf("RGB: emitting red...\n");
			messageOnLCD("      YOU\n       LOSE!", 1);
			printf("You lost! Try again!\n");
			soundNumber = 1;	
			printf("Playing lose sound...\n", ledPattern);
			osSleep(250);
		}
		osSleep(50);
		messageOnLCD("   Scoreboard\n  in terminal.", 1);
		printf("Loading scoreboard...\n", ledPattern);
		printScoreboard(scoreBoard, scorecounter);
		osSleep(100);
		messageOnLCD("PRESS ANY BUTTON\n  FOR NEW GAME", 1);
		while(!(getAnyButton()))
			osSleep(1);
		osSleep(64);
	}
		
	osSleep(100);
}

static void motorProc(void* arg)
{
	for(;;)
	{
		if(motorplay)
			testMotor();
		osSleep(2);
	}
}

static void matrixProc(void* arg)
{
	PINSEL0 |= 0x00001500 ;  //Initiering av SPI
	SPI_SPCCR = 0x08;    
	SPI_SPCR  = 0x60;
	IODIR0 |= SPI_CS;
	startTimer1();
	for(;;)
	{
		printMatrix(ledPattern);
	} 
}

static void soundProc(void* arg)
{
	for(;;)
	{
		if(soundNumber>0)
		{
			soundManager(soundNumber);
			soundNumber=-1;
		}

	}
}

