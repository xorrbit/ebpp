// Evil Bob's Pocket Painter 0.2
//
// Coder   - The Black Frog
// Email   - ebpp@TheBlackFrog.8m.com
// Website - http://www.TheBlackFrog.8m.com
//
// I used Staring Monkey's libraries [time.h and graphics.h], cause i did
// not want to write my own :P you can contact Staring Monkey [StarMonk on
// irc] at StaringMonkey@hotmail.com . You can download his demos off of
// www.gbadev.org.
//
//
// This is a program i made cause i wanted to be able to draw graphics using my gba. 
// It uses mode 3, which is a linear [1D] array of 38400 [240*160] 15 bit color data.
// You draw [in drawing mode] by pressing A to blit the A color, and B to blit the B color.
// You can move the currently selected tool by pressing directions on the d-pad. If you press
// start, you'll get the tool select screen. the tools are:
// 1- pencil ==== blit a one pixel of the button color
// 2- brush ===== blit a 2x2 pixels of the button color
// 3- eraser ==== blit a 2x2 pixels of white
// 4- line ====== press A to set the starting point, then move the cursor, and press A again to
//                set the end point, and then press B to blit the line of colorA.
// 5- rectangle = press A to set one corner, A again to set the other corner, then B to blit
//                a FILLED rectangle of colorA. 
// 6- circle ==== press A to set the center, then press A again for the radius, then press B 
//                to blit a circle with a width of 1 pixel of colorA.
// Also, if you press L on the tools screen, it will save the current picture to SRAM, and if
// you press R, it will load the picture from SRAM. press stat to return to drawing mode. If
// Now for color edit mode. To switch to color edit mode, press select. You start in drawing mode
// btw and to get to tool select mode, press start.
// In color mode, the currently edited color button [A or B] has the color designated to it below
// the A or B in the left there. You can switch the currently edited color by pressing the button
// you want to edit. [only A an B for now] You can edit the Red, Green and Blue values of the color
// by using the sliders on the right. The currently selected slider is white and the other two
// are black. To switch between sliders, press up and down accordingly. To slide the currently
// selected slider, press left to decrement and right to increment. The values for each color can be
// from 0 - 31. that gives you 32*32*32, or 32768 different possible colors. The value [0-31]
// is also displayed in a box to the right of each color. when you're done changing the colors,
// press select to switch back to drawing mode and try out your colors. You can also (while in
// drawing mode) press L to speed up the tool, and R to slow down.
// 
// What i want to include in later versions:
//
// - A program that'll let you change the SRAM into a .tiff
// - more tools such as paint bucket, spray painter, etc.
// - zooming, scaling, rotation [all in mode 3]
// - link support for 2 people editing the same image at the same time. That would rule.
// - some other kinda peripheal support, such as GB printer or something
// - sound of some kind. like a "thud" when you try to go past the edge of the screen, etc.
// - perhaps multibooting for a 4 player drawfest type thingy
//
// History:
//
// - EBPP 0.2 released September 23, 2001. New Features:
//	- Added:
//		- the brush, eraser, line, rectangle and circle tools
//		- SRAM saving
//		- changed the color edit screen
//	- Bug Fixes:
//		- Only draw on half the screen, and the pen would go off the bottom of the screen
//		  because i mixed up x and y [reported by CapNpants]
//		- L and R keys were reversed
//      - Removal:
//		- L an R keys in drawing mode to change tool speed
// - EBPP 0.1 released september 8, 2001. New features:
//	Added:
//	 	- a "pen" that can be speeded up/down
//		- two colors simultaniously with A and B
//		- edit color mode set the r,g and b for the A and B buttons [32768 possible colors]
// - EBPP 0.05 released september 5, 2001. Features:
//	Added:
//		- draw red on a blue background using the d-pad
//		- press select for title screen / color edit teaser
//
// Open Source:
//
// Everything i make is open source. that does not mean you can use my code as your own. it means
// that you can use it if you give appropriate credit. thats all i ask. if you use some of my code
// just put that some code was borrowed from The Black Frog [TheBlackFrog.8m.com] in the source if
// its open source, and if its not, put it in the ROM itself. heck, put it in the ROM anyway :)
// I commented the source as best as possible, but i never comment while coding, only when i release
// so it might not make sense sometimes [to you or me], but it works.
//
// Bugs:
//
// If you find any bugs with this program, and it is the latest release [check my site], please
// email me at ebpp@TheBlackFrog.8m.com with everything you know about the bug, and you'll get your
// name in the source. 
//
// Other:
//
// All my demos and programs are availible with full source from www.TheBlackFrog.8m.com
// That is also the home of Evil Bob's Pocket Painter and you can find out more about it [like
// why i named it Evil Bob's Pocket Painter :P] by visiting my site.
//
// If you want to contact me, i an usually in #gbadev on EFNET between 8 pm until 10 pm [GMT -6]
// every weekday night. Otherwise email me: ebpp@TheBlackFrog.8m.com
//
// Greetz:
//
// Dovoto, Joat, JayC, Loco-san, SlasherX, 
// Staring Monkey, TwinD, Fett, Larsby, Zer0,
// jtafk, #gbadev on efnet, and #BlackFrog on EFNET
//

#define START_OF_DELAY 20
#define TIME_TO_WAIT 120


#include "gba.h"        // eloist's GBA header with many defines
#include "keypad.h"     // dovoto's keypad defines
#include "screenmode.h" // dovoto's screen mode defines
#include "sprite.h"     // dovoto's sprite defines

#include <math.h>
#include <stdlib.h>

#include "graphics.h"
#include "time.h"

#include "coloredit.h" // EditColor(); screen made with bimbo
#include "filemenu.h"
#include "objects.h"   // Sprites include file created with dovoto's pcx2sprite and my fixh program. 

#define RGB(r,g,b) ((r)+((g)<<5)+((b)<<10))  // macro to convert 3 values [R, G, B] into a single 16 bit int that is used in mode 3, Each Red, Green and Blue is from 0-31 which gives up 32*32*32 or 32, 768 possible colors

u8 tenslider[41], oneslider[41], pen=0, linelength=0; // tenslider stores whats in the tens postition of the numbers 0-41, oneslider stores the ones. its used so i know what digit to dislay in the box next to the Red, Green and Blue sliders.

u16 pic[240*160], colorA=RGB(0,0,31), colorB=RGB(0,31,0), colorbg=RGB(31,31,31), color=RGB(31,31,31); // pic is used to store the picture that was drawn when you press select, so it can load back up when you're done switching drawing colors. color holds the current drawing color

u16* BMPOBJData =(u16*)0x6014000; // a pointer to halfway point in charecter memory [cause bitmap modes take up the first half so you only have half the amount of character memory in bitmap modes

#define SRAM 0x0E000000

void FillBG(u16 c2); // Fills the Screen with color c2
void CopyOAM(void); // Copys the info in sprites[] to OAM. in this way you first set all your attributes in sprites[X].attributeY and then copy them, DONT FORGET TO COPY THEM AFTER
void ClearOAM(void); // Clears OAM [object attribute memory] use only at start or all object attributes will be lost
void EditColor(void); // brings up the color editing screen. 
void FileMenu(void);
void WriteSRAM(u16 offset, u8 data);
u8   ReadSRAM(u16 offset);

void C_Entry(void) // main entry point defined in boot.asm
{
        u8 y=80, ones, tens, delay=START_OF_DELAY, xtemp, ytemp, xtemp2, ytemp2, linelength; // xloop is looping variable, x is the current x position of your "pen", y is y position of "pen", yy and xx are temp variables for if you move the pen to the screen edge
	u8 pen5x, pen5y, tempx, tempy, tempo; // i really have to optimze theese goddam variables someday :P
        u16 x=120, temp;
	SetMode(MODE_3 | BG2_ENABLE | OBJ_ENABLE | OBJ_MAP_1D); // Sets the screen to mode 3 [a 240*160 array of 15-bit colors [5 bits per R,G,B = 0-31 each]], turns on background 2 [the only one accessible in bitmap mode], turns on sprites, sets sprites access mode to 1d 

	for(temp=0;temp<256;temp++) // loop for 256 times [0-255]
		OBJPaletteMem[temp] = objectsPalette[temp]; //copys sprite palette into the correct memory location
	for(temp=0;temp<(objects_WIDTH*objects_HEIGHT);temp++) // loop for [8*272] which, if you look at objects.pcx, is the size of all my sprites
		BMPOBJData[temp] = objectsData[temp]; // copy sprites into charecter [sprite] memory
	for(tens=0;tens<4;tens++) // loops thru the tens
		for(ones=0;ones<10;ones++) // loops thru the ones
		{
			tenslider[(tens*10)+ones] = tens; //sets the correct values to use later on in the color editor so that i know what to set the digits next to the sliders to
			oneslider[(tens*10)+ones] = ones; // ^
		}
        WaitForVSync();
	ClearOAM(); // clear OAM

	sprites[0].attribute0 = COLOR_256 | SQUARE | 79; // sets up the pen
	sprites[0].attribute1 = SIZE_8 | 119; 
	sprites[0].attribute2 = 512; // makes it the pencil

	sprites[1].attribute0 = COLOR_256 | TALL | 9; // red slider 
	sprites[1].attribute1 = SIZE_8 | 240; // sets it to 140 [offscreen] because we dont start in color editor
	sprites[1].attribute2 = 514; // makes it the black slider
	
	sprites[2].attribute0 = COLOR_256 | TALL | 24; // green Slider 
	sprites[2].attribute1 = SIZE_8 | 240; // sets it to 140 [offscreen] because we dont start in color editor
	sprites[2].attribute2 = 514; // makes it the black slider

	sprites[3].attribute0 = COLOR_256 | TALL | 39; // Blue Slider
	sprites[3].attribute1 = SIZE_8 | 240; // sets it to 140 [offscreen] because we dont start in color editor
	sprites[3].attribute2 = 514; // makes it the black slider

	sprites[4].attribute0 = COLOR_256 | TALL | 10; // Red 1st digit
	sprites[4].attribute1 = SIZE_8 | 240; // sets it to 140 [offscreen] because we dont start in color editor
	sprites[4].attribute2 = 522; // 0
	sprites[5].attribute0 = COLOR_256 | TALL | 10; // Red 2nd digit
	sprites[5].attribute1 = SIZE_8 | 240; // sets it to 140 [offscreen] because we dont start in color editor
	sprites[5].attribute2 = 522; // 0


	sprites[6].attribute0 = COLOR_256 | TALL | 25; // Green 1st digit
	sprites[6].attribute1 = SIZE_8 | 240; // sets it to 140 [offscreen] because we dont start in color editor
	sprites[6].attribute2 = 522; // 0
	sprites[7].attribute0 = COLOR_256 | TALL | 25; // Green 2nd digit
	sprites[7].attribute1 = SIZE_8 | 240; // sets it to 140 [offscreen] because we dont start in color editor
	sprites[7].attribute2 = 522; // 0


	sprites[8].attribute0 = COLOR_256 | TALL | 40; // Blue 1st digit
	sprites[8].attribute1 = SIZE_8 | 240; // sets it to 140 [offscreen] because we dont start in color editor
	sprites[8].attribute2 = 522; // 0
	sprites[9].attribute0 = COLOR_256 | TALL | 40; // Blue 2nd digit
	sprites[9].attribute1 = SIZE_8 | 240; // sets it to 140 [offscreen] because we dont start in color editor
	sprites[9].attribute2 = 522; // 0

	sprites[10].attribute0 = COLOR_256 | SQUARE | 160;
	sprites[10].attribute1 = SIZE_16 | HORIZONTAL_FLIP | 0;
	sprites[10].attribute2 = 562;
	sprites[11].attribute0 = COLOR_256 | SQUARE | 160;
	sprites[11].attribute1 = SIZE_16 | 111;
	sprites[11].attribute2 = 562;

	sprites[12].attribute0 = COLOR_256 | SQUARE | 160;
	sprites[12].attribute1 = SIZE_8 | 240;
	sprites[12].attribute2 = 562;
	sprites[13].attribute0 = COLOR_256 | SQUARE | 160;
	sprites[13].attribute1 = SIZE_8 | 240;
	sprites[13].attribute2 = 562;

        WaitForVSync();
	CopyOAM(); // Copy all the attributes i just set into OAM
	FillBG(colorbg); // sets the color of the screen to Blue [0 red, 0 green, 31 blue]
	ones=0;
	delay=START_OF_DELAY;
	while(!0) // loops forever
	{
		if(!((*KEYS) & KEY_UP)) // if the up key on the d pad is pressed
			if(y>0)         // makes sure you wont draw off the screen
			{
				if(pen5y==0)
					y--; // decrements the y value of the pen
				else 
				{
					tempx=x;
					x=pen5x;
					if(y==pen5y)
					{
						y=tempy;
						pen5y=tempy;
					}
					y--;
				}
				WaitBlanks(delay); // waits delay/60ths of a second. delay is adjusted by the L and R butttons.
			}
		if(!((*KEYS) & KEY_DOWN)) // if the down key is pressed
			if(y<159) //makes sure you wont draw off the screen
			{
				if(pen5y==0)
					y++; // increments the y value of the screen
				else
				{
					tempx=x;
					x=pen5x;
					if(y==pen5y)
					{
						y=tempy;
						pen5y=tempy;
					}
					y++;
				}
				WaitBlanks(delay); // waits delay/60ths of a second. delay is adjusted by the L and R butttons.
			}
		if(!((*KEYS) & KEY_LEFT)) // if left is pressed
			if(x>0) // makes sure you wont draw off the scrren
			{
				if(pen5x==0)
					x--; // decrements the x value of the "pen"
				else 
				{
					tempy=y;
					y=pen5y;
					if(x==pen5x)
					{
						x=tempx;
						pen5x=tempx;
					}
					x--;
				}
				WaitBlanks(delay); // waits delay/60ths of a second. delay is adjusted by the L and R butttons.
			}
		if(!((*KEYS) & KEY_RIGHT)) // ir right is pressed
			if(x<239) // make sure you wont draw off the screen
			{
				if(pen5x==0)
					x++; // increments x
				else
				{
					tempy=y;
					y=pen5y;
					if(x==pen5x)
					{
						x=tempx;
						pen5x=tempx;
					}
					x++;
				}
				WaitBlanks(delay); // waits delay/60ths of a second. delay is adjusted by the L and R butttons.
			}
		if(!((*KEYS) & KEY_L))
		{
			if(delay>20)
				delay-=10;
			WaitBlanks(30);
		}
		if(!((*KEYS) & KEY_R))
		{
			if(delay<240)
				delay+=10;
			WaitBlanks(30);
		}		
		if(!((*KEYS) & KEY_A)) // if your press A,
			color=colorA;  // make the color to be plotted be color A
		else if(!((*KEYS) & KEY_B)) // if you press B,
			color=colorB;       // make the color being blitted be color B
		else color=colorbg; // otherwiese make it the BG color
		if(!((*KEYS) & KEY_SELECT)) // if you press select
		{
			for(temp=0;temp<(240*160);temp++)       // loads the current picture into an array, pic
				pic[temp] = VideoBuffer[temp];  // ^
			EditColor(); // load the editcolor foxion
                        WaitForVSync();
			for(temp=0;temp<(240*160);temp++)       // loads pic back onto the screen
				VideoBuffer[temp] = pic[temp];  // ^
			sprites[1].attribute1 = SIZE_8 | 240; // sets it to 140 [offscreen] because we dont start in color editor
			sprites[2].attribute1 = SIZE_8 | 240; // sets it to 140 [offscreen] because we dont start in color editor
			sprites[3].attribute1 = SIZE_8 | 240; // sets it to 140 [offscreen] because we dont start in color editor
			sprites[4].attribute1 = SIZE_8 | 240; // sets it to 140 [offscreen] because we dont start in color editor
			sprites[5].attribute1 = SIZE_8 | 240; // sets it to 140 [offscreen] because we dont start in color editor
			sprites[6].attribute1 = SIZE_8 | 240; // sets it to 140 [offscreen] because we dont start in color editor
			sprites[7].attribute1 = SIZE_8 | 240; // sets it to 140 [offscreen] because we dont start in color editor
			sprites[8].attribute1 = SIZE_8 | 240; // sets it to 140 [offscreen] because we dont start in color editor
			sprites[9].attribute1 = SIZE_8 | 240; // sets it to 140 [offscreen] because we dont start in color editor
			sprites[12].attribute1 = SIZE_8 | 240; // sets it to 140 [offscreen] because we dont start in color editor
			sprites[13].attribute1 = SIZE_8 | 240; // sets it to 140 [offscreen] because we dont start in color editor
			ones=0;
		}
		if(!((*KEYS) & KEY_START)) // if you press start
		{
			for(temp=0;temp<(240*160);temp++)
				pic[temp] = VideoBuffer[temp];
			FileMenu();
                        WaitForVSync();
			for(temp=0;temp<(240*160);temp++)
				VideoBuffer[temp] = pic[temp];
			sprites[10].attribute0 = COLOR_256 | SQUARE | 160;
			sprites[11].attribute0 = COLOR_256 | SQUARE | 160;
			if(pen==0)
				sprites[0].attribute2 = 512;
			if(pen==1)
				sprites[0].attribute2 = 570;
			if(pen==2)
				sprites[0].attribute2 = 572;
			if(pen==3)
				sprites[0].attribute2 = 574;
			if(pen==4)
				sprites[0].attribute2 = 574;
			if(pen==5)
				sprites[0].attribute2 = 574;
			ones=0;
		}
                if(pen==0)
                {
                	sprites[0].attribute0 = COLOR_256 | SQUARE | y;
                	sprites[0].attribute1 = SIZE_8 | x;
			if(color != colorbg) // plots the pixel only if you are pressing A or B, otherwise just moves the "pen"
			{
				WaitForVSync();
				PlotPixel(x,y,color); //plots the pixel!
			}
		}
		if(pen==1)
		{
                	sprites[0].attribute0 = COLOR_256 | SQUARE | y;
                	sprites[0].attribute1 = SIZE_8 | x;
			if(color != colorbg)
			{
	                        WaitForVSync();
				PlotPixel(x+1,y,color);
				PlotPixel(x,y,color);
				PlotPixel(x,y+1,color);
				PlotPixel(x+1,y+1,color);
			}
		}
		if(pen==2)
		{
		       	sprites[0].attribute0 = COLOR_256 | SQUARE | y;
                	sprites[0].attribute1 = SIZE_8 | x;
			if(color != colorbg)
			{
                		WaitForVSync();
				PlotPixel(x+1,y,colorbg);
				PlotPixel(x,y,colorbg);
				PlotPixel(x,y+1,colorbg);
				PlotPixel(x+1,y+1,colorbg);
			}
		}
		if(pen==3)
		{
               		sprites[0].attribute0 = COLOR_256 | SQUARE | y-2;
               		sprites[0].attribute1 = SIZE_8 | x-2;
			if(color==colorA)
			{
				if(ones==0)
				{
					sprites[12].attribute0 = COLOR_256 | SQUARE | y-2;
					sprites[12].attribute1 = SIZE_8 | x-2;
					sprites[12].attribute2 = 576;
					xtemp=x;
					ytemp=y;
					ones=1;
				}
				else if(ones==1)
				{
					sprites[13].attribute0 = COLOR_256 | SQUARE | y-2;
					sprites[13].attribute1 = SIZE_8 | x-2;
					sprites[13].attribute2 = 576;
					xtemp2=x;
					ytemp2=y;
					ones=0;
				}
                        WaitForVSync();
			CopyOAM();
			while(!((*KEYS) & KEY_A)) {};
			}
			if(color==colorB)
			{
				sprites[12].attribute1 = SIZE_8 | 240;
				sprites[13].attribute1 = SIZE_8 | 240;
                                WaitForVSync();
				CopyOAM();
				Line(xtemp,ytemp,xtemp2,ytemp2,colorA);
			}
		}

		if(pen==4)
		{
               		sprites[0].attribute0 = COLOR_256 | SQUARE | y-2;
               		sprites[0].attribute1 = SIZE_8 | x-2;
			if(color==colorA)
			{
				if(ones==0)
				{
					sprites[12].attribute0 = COLOR_256 | SQUARE | y-2;
					sprites[12].attribute1 = SIZE_8 | x-2;
					sprites[12].attribute2 = 576;
					xtemp=x;
					ytemp=y;
					ones=1;
				}
				else if(ones==1)
				{
					sprites[13].attribute0 = COLOR_256 | SQUARE | y-2;
					sprites[13].attribute1 = SIZE_8 | x-2;
					sprites[13].attribute2 = 576;
					xtemp2=x;
					ytemp2=y;
					ones=0;
				}
                        WaitForVSync();
			CopyOAM();
			while(!((*KEYS) & KEY_A)) {};
			}
			if(color==colorB)
			{
				sprites[12].attribute1 = SIZE_8 | 240;
				sprites[13].attribute1 = SIZE_8 | 240;
                                WaitForVSync();
				CopyOAM();
				if(xtemp>xtemp2)
				{
					tempo=xtemp2;
					xtemp2=xtemp;
					xtemp=tempo;
				}
				if(ytemp>ytemp2)
				{
					tempo=ytemp2;
					ytemp2=ytemp;
					ytemp=tempo;
				}	
				FillBox(xtemp,ytemp,xtemp2,ytemp2,colorA);
			}
		}


		if(pen==5)
		{
               		sprites[0].attribute0 = COLOR_256 | SQUARE | y-2;
               		sprites[0].attribute1 = SIZE_8 | x-2;
			if(color==colorA)
			{
				if(ones==0)
				{
					sprites[12].attribute0 = COLOR_256 | SQUARE | y-2;
					sprites[12].attribute1 = SIZE_8 | x-2;
					sprites[12].attribute2 = 576;
					xtemp=x;
					ytemp=y;
					pen5x=x;
					pen5y=y;
					tempx=x;
					tempy=y;
					ones=1;
				}
				else if(ones==1)
				{
					sprites[13].attribute0 = COLOR_256 | SQUARE | y-2;
					sprites[13].attribute1 = SIZE_8 | x-2;
					sprites[13].attribute2 = 576;
					xtemp2=x;
					ytemp2=y;
					ones=0;
				}
                        WaitForVSync();
			CopyOAM();
			while(!((*KEYS) & KEY_A)) {};
			}
			if(color==colorB)
			{
				sprites[12].attribute1 = SIZE_8 | 240;
				sprites[13].attribute1 = SIZE_8 | 240;
				if(xtemp==xtemp2)
				{
					if(ytemp>ytemp2)
						linelength=(ytemp-ytemp2);
					if(ytemp2>ytemp)
						linelength=(ytemp2-ytemp);
				}
				else if(ytemp==ytemp2)
				{
					if(xtemp>xtemp2)
						linelength=(xtemp-xtemp2);
					if(xtemp2>xtemp)
						linelength=(xtemp2-xtemp);
				}
                                WaitForVSync();
				Circle(xtemp,ytemp,linelength,colorA);

			}
		}
                WaitForVSync();
		CopyOAM(); // copys sprites
	}	

}


void EditColor() // editcolor funxion
{
	u8 xloop, yloop, tempvar1, rgb, ab=0; // variables
	u16 temp, R, G, B;                    // variables
        WaitForVSync();
	for(xloop=0;xloop<240;xloop++) //loads the data from coloredit.h onto the screen
		for(yloop=0;yloop<160;yloop++)
			VideoBuffer[yloop*240+xloop] = coloreditBitmap[yloop*240+xloop];
	if(ab==0)
	{
		temp=31;              // theese extract the Red, green and blue data from the 15 bit values used in mode 3 into 3 5 bit values from 0-31
		R=colorA & temp;
		temp=992;
		G=(colorA & temp)>>5;
		temp=31744;
		B=(colorA & temp)>>10;
	}
	if(ab==1)
	{
		temp=31;           // theese extract the Red, green and blue data from the 15 bit values used in mode 3 into 3 5 bit values from 0-31
		R=colorB & temp;
		temp=992;
		G=(colorB & temp)>>5;
		temp=31744;
		B=(colorB & temp)>>10;
	}

	sprites[0].attribute1 = SIZE_8 | 240; // sets the pen to offscreen, cause in editing colors now, not drawing
	sprites[1].attribute1 = SIZE_8 | 43;  // puts the red slider to the 0 position.
	sprites[2].attribute1 = SIZE_8 | 43;  //puts Green to 0
	sprites[3].attribute1 = SIZE_8 | 43;  //puts the Blue Slider to the 0 position
	
	sprites[4].attribute2 = ((tenslider[R]*4)+522); // makes the digit sprites point to the right character in memory
	sprites[5].attribute2 = ((oneslider[R]*4)+522); // ^

	sprites[4].attribute1 = SIZE_8 | 178; // makes the digit sprites come back on screen
	sprites[5].attribute1 = SIZE_8 | 185; // ^

	sprites[6].attribute2 = ((tenslider[G]*4)+522); 
	sprites[7].attribute2 = ((oneslider[G]*4)+522);

	sprites[6].attribute1 = SIZE_8 | 178;
	sprites[7].attribute1 = SIZE_8 | 185;

	sprites[8].attribute2 = ((tenslider[B]*4)+522);
	sprites[9].attribute2 = ((oneslider[B]*4)+522);

	sprites[8].attribute1 = SIZE_8 | 178;
	sprites[9].attribute1 = SIZE_8 | 185;
 
        WaitForVSync(); // waits for the scren to finish drawing
	CopyOAM(); // copys Object Attribute Memory
	tempvar1=0; 
	rgb=0; // sets the current selected color to 0 [red]
	
	while(tempvar1==0)
	{
		if(!((*KEYS) & KEY_A))
			if(ab==1)
			{
				ab=0;
				temp=31;
				R=colorA & temp;
				temp=992;
				G=(colorA & temp)>>5;
				temp=31744;
				B=(colorA & temp)>>10;
			}
		if(!((*KEYS) & KEY_B))
			if(ab==0)
			{
				ab=1;
				temp=31;
				R=colorB & temp;
				temp=992;
				G=(colorB & temp)>>5;
				temp=31744;
				B=(colorB & temp)>>10;
			}
		if(!((*KEYS) & KEY_UP))
		{
			if(rgb==0) // Red
			{
				rgb=2;
			}
			else if(rgb==1) // Green
			{
				rgb=0;
			}
			else if(rgb==2) // Blue
			{
				rgb=1;
			}
			WaitBlanks(TIME_TO_WAIT);
		}
		if(!((*KEYS) & KEY_DOWN))
		{
			if(rgb==0) // Red
			{
				rgb=1;
			}
			else if(rgb==1) // Green
			{
				rgb=2;
			}
			else if(rgb==2) // Blue
			{
				rgb=0;
			}
			WaitBlanks(TIME_TO_WAIT); // this line sends 1000 emails to those goddam idiots who made the "internet threasure chest" infomercial!
		}
		if(!((*KEYS) & KEY_LEFT))
		{
			if(rgb==0) // Red
				if(R>0)
					R-=1;
			if(rgb==1) // Green
				if(G>0)
					G-=1;
			if(rgb==2) // Blue
				if(B>0)
					B-=1;
			WaitBlanks(TIME_TO_WAIT);
		}
		if(!((*KEYS) & KEY_RIGHT)) // if(right+right!wrong) right=wrong;
		{
			if(rgb==0) // Red
				if(R<31)
					R+=1;
			if(rgb==1) // Green
				if(G<31)
					G+=1;
			if(rgb==2) // Blue
				if(B<31)
					B+=1;
			WaitBlanks(TIME_TO_WAIT);
		}
		if(!((*KEYS) & KEY_SELECT))
			tempvar1=1;


	sprites[1].attribute1 = SIZE_8 | ((R*4)+43);
	sprites[2].attribute1 = SIZE_8 | ((G*4)+43);
	sprites[3].attribute1 = SIZE_8 | ((B*4)+43);

	sprites[1].attribute2 = 514;
	sprites[2].attribute2 = 514;
	sprites[3].attribute2 = 514;

	sprites[rgb+1].attribute2 = 518;
	
	sprites[4].attribute2 = ((tenslider[R]*4)+522);
	sprites[5].attribute2 = ((oneslider[R]*4)+522);

	sprites[6].attribute2 = ((tenslider[G]*4)+522);
	sprites[7].attribute2 = ((oneslider[G]*4)+522);

	sprites[8].attribute2 = ((tenslider[B]*4)+522);
	sprites[9].attribute2 = ((oneslider[B]*4)+522);
	if(ab==0)
	{
		colorA = (B<<10) | (G<<5) | R;
                WaitForVSync();
		for(xloop=200;xloop<(200+15);xloop++)
			for(yloop=20;yloop<(20+15);yloop++)
				VideoBuffer[yloop*240+xloop] = colorA;
                WaitForVSync();
		for(xloop=221;xloop<(221+15);xloop++)
			for(yloop=20;yloop<(20+15);yloop++)
				VideoBuffer[yloop*240+xloop] = RGB(31,31,31); // 640k is enuf for me!
	}
	if(ab==1)
	{
		colorB = (B<<10) | (G<<5) | R;
                WaitForVSync();
		for(xloop=221;xloop<(221+15);xloop++)
			for(yloop=20;yloop<(20+15);yloop++)
				VideoBuffer[yloop*240+xloop] = colorB;
                WaitForVSync();
		for(xloop=200;xloop<(200+15);xloop++)
			for(yloop=20;yloop<(20+15);yloop++)
				VideoBuffer[yloop*240+xloop] = RGB(31,31,31);
	}
        WaitForVSync();
	CopyOAM();
	}
}

void FileMenu(void)
{
	u8 xloop, yloop, a, b;
	u16 temp=0;
        WaitForVSync();
	for(xloop=0;xloop<240;xloop++) //loads the data from filemenu.h onto the screen
		for(yloop=0;yloop<160;yloop++)
			VideoBuffer[yloop*240+xloop] = filemenuBitmap[yloop*240+xloop];
	while(!((*KEYS) & KEY_START));
	sprites[0].attribute1 = SIZE_8 | 240;
	sprites[12].attribute1 = SIZE_8 | 240;
	sprites[13].attribute1 = SIZE_8 | 240;
        WaitForVSync();
	CopyOAM();
	while(temp==0)
	{
		if(!((*KEYS) & KEY_UP))
		{
			if(pen>0)
			{
				pen--;
			}
			else pen=5;
			WaitBlanks(TIME_TO_WAIT);
		}
		if(!((*KEYS) & KEY_DOWN)) // visit dcvision.com right now!
		{
			if(pen<5)
			{
				pen++;
			}
			else pen=0;
			WaitBlanks(TIME_TO_WAIT);
		}
		if(!((*KEYS) & KEY_START))
			temp=1;
		if(!((*KEYS) & KEY_L))
		{
			for(temp=0;temp<(240*160);temp++)
			{
				b = pic[temp] & 0xff;
				a = (pic[temp] >> 8) & 0xff;
				*(u8 *)(SRAM + (temp*2)) = a;
				*(u8 *)(SRAM + ((temp*2)+1)) = b; // clevercam!
			}
			temp=0;
		}
		else if(!((*KEYS) & KEY_R))
		{
			for(temp=0;temp<(240*160);temp++)
			{
				b = *(u8 *)(SRAM + (temp*2));
				a = *(u8 *)(SRAM + ((temp*2)+1));
				pic[temp] = a+(b<<8);
			}
			temp=0;
		}
		if(pen==0)
		{
			sprites[10].attribute1 = SIZE_16 | HORIZONTAL_FLIP | 34-15;
			sprites[11].attribute1 = SIZE_16 | 92;
		}
		if(pen==1)
		{
			sprites[10].attribute1 = SIZE_16 | HORIZONTAL_FLIP | 35-15;
			sprites[11].attribute1 = SIZE_16 | 90;
		}
		if(pen==2)
		{
			sprites[10].attribute1 = SIZE_16 | HORIZONTAL_FLIP | 31-15;
			sprites[11].attribute1 = SIZE_16 | 95;
		}
		if(pen==3)
		{
			sprites[10].attribute1 = SIZE_16 | HORIZONTAL_FLIP | 43-15;
			sprites[11].attribute1 = SIZE_16 | 82;
		}
		if(pen==4)
		{
			sprites[10].attribute1 = SIZE_16 | HORIZONTAL_FLIP | 15-15;
			sprites[11].attribute1 = SIZE_16 | 110;
		}
		if(pen==5)
		{
			sprites[10].attribute1 = SIZE_16 | HORIZONTAL_FLIP | 33-15;
			sprites[11].attribute1 = SIZE_16 | 91;
		}
		sprites[10].attribute0 = COLOR_256 | SQUARE | ((pen*23)+19);
		sprites[11].attribute0 = COLOR_256 | SQUARE | ((pen*23)+19); // ho-hum
                WaitForVSync();
		CopyOAM();
	}

}

void ClearOAM(void)
{
	u8 loop;
	for(loop=0;loop<128;loop++)
	{
		sprites[loop].attribute0 = 160;
		sprites[loop].attribute1 = 240;
	}
}

void FillBG(u16 c2)
{
	u8 xtemp,ytemp;
	for(xtemp=0;xtemp<240;xtemp++)
		for(ytemp=0;ytemp<160;ytemp++)
			VideoBuffer[((ytemp)*240)+(xtemp)] = c2;
}

void CopyOAM(void) 
{
	u16 loop;
	u16* temp;
	temp = (u16*)sprites;
	for(loop = 0; loop < 128*4; loop++)
		OAM[loop] = temp[loop];
}

void WriteSRAM(u16 offset, u8 data)
{
//	u8 lowbits=0, highbits=0;
//	if((data & 0xff)==data)
//	{
		*(u8 *)(SRAM + offset) = data;		
//	}
//	else
//	{
//		lowbits = data & 0xff;
//		highbits = (data >> 8) & 0xff;
//		*(u8 *)(SRAM + (offset*2)) = lowbits;
//		*(u8 *)(SRAM + ((offset*2)+1)) = highbits;
//	}
}

u8 ReadSRAM(u16 offset)
{
	u8 sramdata;
	sramdata = *(u8 *)(SRAM + offset);
	return sramdata;
}

	


	
	