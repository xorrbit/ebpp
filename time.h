/*****************************\
* 	time.h
*	by staringmonkey
*	Last modified on 8/22/01
\*****************************/

#ifndef TIMERS_H
#define TIMERS_H

#define PI 3.141592654

#define BIT00 1
#define BIT01 2
#define BIT02 4
#define BIT03 8
#define BIT04 16
#define BIT05 32
#define BIT06 64
#define BIT07 128
#define BIT08 256
#define BIT09 512
#define BIT10 1024
#define BIT11 2048
#define BIT12 4096
#define BIT13 8192
#define BIT14 16384
#define BIT15 32768

///////////////////Definitions////////////////////
#define FREQUENCY_0		0x00
#define FREQUENCY_64	BIT00
#define FREQUENCY_256	BIT01
#define FREQUENCY_1024	BIT00 | BIT01

#define TIMER_CASCADE	BIT02
#define TIMER_IRQ		BIT06
#define TIMER_ENABLE	BIT07

////////////////Function Prototypes/////////////////
void WaitBlanks(int numBlanks);
void WaitTime(int seconds, int milliSeconds);
void WaitForVSync(void);

//////////////////WaitBlanks///////////////////////
void WaitBlanks(int numBlanks)
{
	int i = 0;

	while(i < numBlanks)
	{
		WaitForVSync();
		i++;
	}
}

///////////////////WaitTime////////////////////
void WaitTime(int seconds, int milliSeconds)
{
	//Enable timers
	REG_TM2CNT = FREQUENCY_256 | TIMER_ENABLE;
	REG_TM3CNT = TIMER_CASCADE | TIMER_ENABLE;

	//Wait until ? seconds have passed
	while(REG_TM3D < seconds)
	{
	}

	//Zero out base/millisecond timer
	REG_TM2D = 0;

	//1 millisecond = max register value (65536) divided by the # milliseconds in a second (1000)	
	//Wait until ? milliseconds have passed
	while(REG_TM2D * 65536/1000 < milliSeconds)
	{
	}

	//Disable timers
	REG_TM2CNT = 0;
	REG_TM3CNT = 0;
	
	//Zero out timer values
	REG_TM2D = 0;
	REG_TM3D = 0;
}

///////////////WaitForVSync////////////////////////
void WaitForVSync(void)
{
	__asm 
	{
		mov 	r0, #0x4000006  //0x4000006 is vertical trace counter; when it hits 160					 //160 the vblanc starts
		scanline_wait:	       	//the vertical blank period has begun. done in asm just 
								//because:)
		ldrh	r1, [r0]
		cmp	r1, #160
		bne 	scanline_wait
	}
}

#endif
