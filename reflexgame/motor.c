#include "../pre_emptive_os/api/osapi.h"
#include "../pre_emptive_os/api/general.h"
#include <printf_P.h>
#include <lpc2xxx.h>


void testMotor(void)
{
	static tU32 stepmotorSteps[4] = {(1<<12 | 1<<21), 1<<21, 0x00, 1<<12};  //P0.21 and P0.12 are used to contol the stepper motor
	static tU8 stepmotorIndex = 0;
	tU8 i;
	IODIR0 |= (1<<12 | 1<<21);
	IOCLR0  = stepmotorSteps[0];
	for(i=0; i<5*3; i++)
	{  
		//update to new step (forward)
		stepmotorIndex = (stepmotorIndex + 1) & 0x03;

		//output new step
		IOCLR0 = stepmotorSteps[0];
		IOSET0 = stepmotorSteps[stepmotorIndex];
		osSleep(5);
	}
}
