// Evil Bob's Pocket Painter 0.1
//
// Coder   - The Black Frog
// Email   - ebpp@TheBlackFrog.8m.com
// Website - http://www.TheBlackFrog.8m.com
//
// This is a program i made cause i wanted to be able to draw graphics using my gba. 
// It uses mode 3, which is a linear [1D] array of 38400 [240*160] 15 bit color data.
// You draw [in drawing mode] by pressing A to blit the A color, and B to blit the B color.
// You can move the "pen" [a little box atm] by pressing directions on the d-pad. If you press
// L while in drawing mode, the speed fo the "pen" will slow down. If you press R, it'll
// speed up. right now the only tool is a pen, but i plan to have others such as paint bucket,
// circle, line and square tool, some kinda cut and paste system, and watev else people suggest.
// Now for color edit mode. To switch between modes, press select. You start in drawing mode btw.
// In color mode, the currently edited color button [A or B] has the color designated to it below
// the A or B in the left there. You can switch the currently edited color by pressing the button
// you want to edit. [only A an B for now] You can edit the Red, Green and Blue values of the color
// by using the sliders on the right. The currently selected slider is white and the other two
// are black. To switch between sliders, press up and down accordingly. To slide the currently
// selected slider, press left to decrement and right to increment. The values for each color can be
// from 0 - 31. that gives you 32*32*32, or 32768 different possible colors. The value [0-31]
// is also displayed in a box to the right of each color. when you're done changing the colors,
// press select to switch back to drawing mode and try out your colors. It has been tested on
// hardware by JayC, Loco-San and SlasherX, but as i have no FAL i dont know how kewl it is.
// 
// What i want to include in later versions:
//
// - SRAM saving. and also a program that'll let you change the SRAM into a .bmp or .gif.
// - more tools such as line, circle, square, paint bucket, eraser, spray painter, etc.
// - pen size changing, different shape pens, etc.
// - zooming, scaling, rotation [all in mode 3]
// - link support for 2 people editing the same image at the same time. That would rule.
// - some other kinda peripheal support, such as GB printer or something
// - sound of some kind. like a "thud" when you try to go past the edge of the screen, etc.
// - perhaps multibooting for a 4 player drawfest type thingy
//
// History:
//
// - EBPP 0.1 released september 8, 2001. New features:
// 	- a "pen" that can be speeded up/down
//	- two colors simultaniously with A nd B
//	- edit color function working great
// - EBPP 0.05 released september 5, 2001. Features:
//	- draw red on a blue background using the d-pad
//	- press select for title screen / color edit teaser
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
// Dovoto, Joat, JayC, Loco-san, SlasherX, #gbadev on efnet
//

#include "gba.h"        // eloist's GBA header with many defines
#include "keypad.h"     // dovoto's keypad defines
#include "screenmode.h" // dovoto's screen mode defines
#include "sprite.h"     // dovoto's sprite defines

#include "coloredit.h" // EditColor(); screen made with bimbo

#include "objects.h"   // Sprites include file created with dovoto's pcx2sprite and my fixh program. 

#define RGB(r,g,b) ((r)+((g)<<5)+((b)<<10))  // macro to convert 3 values [R, G, B] into a single 16 bit int that is used in mode 3, Each Red, Green and Blue is from 0-31 which gives up 32*32*32 or 32, 768 possible colors

u8 tenslider[41], oneslider[41]; // tenslider stores whats in the tens postition of the numbers 0-41, oneslider stores the ones. its used so i know what digit to dislay in the box next to the Red, Green and Blue sliders.

u16 pic[240*160], colorA=RGB(0,0,31), colorB=RGB(0,31,0), colorbg=RGB(31,31,31), color=RGB(31,31,31); // pic is used to store the picture that was drawn when you press select, so it can load back up when you're done switching drawing colors. color holds the current drawing color

u16* BMPOBJData =(u16*)0x6014000; // a pointer to halfway point in charecter memory [cause bitmap modes take up the first half so you only have half the amount of character memory in bitmap modes

void PlotPixel(u8 xtemp, u8 ytemp, u16 c); // Plots a pixel at coordonate x,y with color c
void WaitForVsync(u8 howmany);  // Waits for (howmany) Vsyncs to occur. If you do a WaitFotVsync(60) it would wait one second. usefull in some way
void FillBG(u16 c2); // Fills the Screen with color c2
void CopyOAM(void); // Copys the info in sprites[] to OAM. in this way you first set all your attributes in sprites[X].attributeY and then copy them, DONT FORGET TO COPY THEM AFTER
void ClearOAM(void); // Clears OAM [object attribute memory] use only at start or all object attributes will be lost
void EditColor(void); // brings up the color editing screen. 

void C_Entry(void) // main entry point defined in boot.asm
{
	u8 x=120, y=80, yy, ones, tens, delay=20; // xloop is looping variable, x is the current x position of your "pen", y is y position of "pen", yy and xx are temp variables for if you move the pen to the screen edge
	u16 xx, temp;
	SetMode(MODE_3 | BG2_ENABLE | OBJ_ENABLE | OBJ_MAP_1D); // Sets the screen to mode 3 [a 240*160 array of 15-bit colors [5 bits per R,G,B = 0-31 each]], turns on background 2 [the only one accessible in bitmap mode], turns on sprites, sets sprites access mode to 1d 

	for(temp=0;temp<256;temp++) // loop for 256 times [0-255]
		OBJPaletteMem[temp] = objectsPalette[temp]; //copys sprite palette into the correct memory location
	for(temp=0;temp<1600;temp++) // loop for 1600 [8*200] which, if you look at objects.pcx, is the size of all my sprites
		BMPOBJData[temp] = objectsData[temp]; // copy sprites into charecter [sprite] memory
	for(tens=0;tens<4;tens++) // loops thru the tens
		for(ones=0;ones<10;ones++) // loops thru the ones
		{
			tenslider[(tens*10)+ones] = tens; //sets the correct values to use later on in the color editor so that i know what to set the digits next to the sliders to
			oneslider[(tens*10)+ones] = ones; // ^
		}

	ClearOAM(); // clear OAM

	sprites[0].attribute0 = COLOR_256 | SQUARE | 79; // sets up the pen
	sprites[0].attribute1 = SIZE_8 | 119; 
	sprites[0].attribute2 = 512; // makes it the box

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
	
	CopyOAM(); // Copy all the attributes i just set into OAM
	FillBG(colorbg); // sets the color of the screen to Blue [0 red, 0 green, 31 blue]
	while(!0) // loops forever
	{
		if(!((*KEYS) & KEY_UP)) // if the up key on the d pad is pressed
			if(y>0)         // makes sure you wont draw off the screen
			{
				y--; // decrements the y value of the pen
				WaitForVsync(delay); // waits delay/60ths of a second. delay is adjusted by the L and R butttons.
			}
		if(!((*KEYS) & KEY_DOWN)) // if the down key is pressed
			if(y<239) //makes sure you wonr draw off the screen
			{
				y++; // increments the y value of the screen
				WaitForVsync(delay); // waits delay/60ths of a second. delay is adjusted by the L and R butttons.
			}
		if(!((*KEYS) & KEY_LEFT)) // if left is pressed
			if(x>0) // makes sure you wont draw off the scrren
			{
				x--; // decrements the x value of the "pen"
				WaitForVsync(delay); // waits delay/60ths of a second. delay is adjusted by the L and R butttons.
			}
		if(!((*KEYS) & KEY_RIGHT)) // ir right is pressed
			if(x<159) // make ssure you wont draw off the screen
			{
				x++; // increments x
				WaitForVsync(delay); // waits delay/60ths of a second. delay is adjusted by the L and R butttons.
			}
		if(!((*KEYS) & KEY_A)) // if your press A,
			color=colorA;  // make the color to be plotted be color A
		else if(!((*KEYS) & KEY_B)) // if you press B,
			color=colorB;       // make the color being blitted be color B
		else color=colorbg; // otherwiese make it the BG color
		if(!((*KEYS) & KEY_L)) // if you press L
			if(delay>1) // if delay is more than 1
			{ 
				delay-=10; // then decrment delay by 10
				WaitForVsync(30); // wait half a second so you dont make it move too quickly/slowly
			}
		if(!((*KEYS) & KEY_R))
			if(delay<200)	
			{
				delay+=10;
				WaitForVsync(30);
			}
		if(!((*KEYS) & KEY_SELECT)) // if you press select
		{
			for(temp=0;temp<(240*160);temp++)       // loads the current picture into an array, pic
				pic[temp] = VideoBuffer[temp];  // ^
			EditColor(); // load the editcolor foxion
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
		}

		if(x==0) // if the pen is at the edge of the screen, make xx be 255, cause a sprite can't be at a coordinate thats negative, so it has to be 255+y [and y has to be negatve]
			xx=512; // look up a line
		else xx=0; // if the pen istn at the edge, make xx be 0
		if(y==0) // loooookkk uuupppp ^^^^^
			yy=255; // kljhd
		else yy=0; // lqowoekr tuypu
		sprites[0].attribute0 = COLOR_256 | SQUARE | (yy+(y-1)); // sets the pen to y-1 cause the pen has to be around the pixel being drawn
		sprites[0].attribute1 = SIZE_8 | (xx+(x-1)); // bLAH
		WaitForVsync(1); // waits for the screen to be drawn
		if(color != colorbg) // plots the pixel only if you are pressing A or B, otherwise just moves the "pen"
			PlotPixel(x,y,color); //plots the pixel!
		CopyOAM(); // copys sprites
	}	

}


void EditColor() // editcolor funxion
{
	u8 xloop, yloop, tempvar1, rgb, ab=0; // variables
	u16 temp, R, G, B;                    // variables
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
 
	WaitForVsync(1); // waits for the scren to finish drawing
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
			WaitForVsync(120);
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
			WaitForVsync(120);
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
			WaitForVsync(30);
		}
		if(!((*KEYS) & KEY_RIGHT))
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
			WaitForVsync(30);
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
	WaitForVsync(1);
	if(ab==0)
	{
		colorA = (B<<10) | (G<<5) | R;
		for(xloop=200;xloop<(200+15);xloop++)
			for(yloop=20;yloop<(20+15);yloop++)
				VideoBuffer[yloop*240+xloop] = colorA;
		for(xloop=221;xloop<(221+15);xloop++)
			for(yloop=20;yloop<(20+15);yloop++)
				VideoBuffer[yloop*240+xloop] = RGB(31,31,31);
	}
	if(ab==1)
	{
		colorB = (B<<10) | (G<<5) | R;
		for(xloop=221;xloop<(221+15);xloop++)
			for(yloop=20;yloop<(20+15);yloop++)
				VideoBuffer[yloop*240+xloop] = colorB;
		for(xloop=200;xloop<(200+15);xloop++)
			for(yloop=20;yloop<(20+15);yloop++)
				VideoBuffer[yloop*240+xloop] = RGB(31,31,31);
	}
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
void PlotPixel(u8 xtemp, u8 ytemp, u16 c)
{
	VideoBuffer[((ytemp)*240)+(xtemp)] = c;
}

void WaitForVsync(u8 howmany)
{
	u8 temp;
	for(temp=0;temp<howmany;temp++)
	{
		__asm 
		{
			mov 	r0, #0x4000006   
			scanline_wait:	       	
			ldrh	r1, [r0]
			cmp	r1, #160
			bne 	scanline_wait
		}	
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
	{
		OAM[loop] = temp[loop];
	}
}