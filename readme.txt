Evil Bob's Pocket Painter 0.2

Coder   - The Black Frog
Email   - ebpp@TheBlackFrog.8m.com
Website - http://www.TheBlackFrog.8m.com

I used Staring Monkey's libraries [time.h and graphics.h], cause i did
not want to write my own :P you can contact Staring Monkey [StarMonk on
irc] at StaringMonkey@hotmail.com . You can download his demos off of
www.gbadev.org.


This is a program i made cause i wanted to be able to draw graphics using my gba. 
It uses mode 3, which is a linear [1D] array of 38400 [240*160] 15 bit color data.
You draw [in drawing mode] by pressing A to blit the A color, and B to blit the B color.
You can move the currently selected tool by pressing directions on the d-pad. If you press
start, you'll get the tool select screen. the tools are:
1- pencil ==== blit a one pixel of the button color
2- brush ===== blit a 2x2 pixels of the button color
3- eraser ==== blit a 2x2 pixels of white
4- line ====== press A to set the starting point, then move the cursor, and press A again to
               set the end point, and then press B to blit the line of colorA.
5- rectangle = press A to set one corner, A again to set the other corner, then B to blit
               a FILLED rectangle of colorA. 
6- circle ==== press A to set the center, then press A again for the radius, then press B 
               to blit a circle with a width of 1 pixel of colorA.
Also, if you press L on the tools screen, it will save the current picture to SRAM, and if
you press R, it will load the picture from SRAM. press stat to return to drawing mode. If
Now for color edit mode. To switch to color edit mode, press select. You start in drawing mode
btw and to get to tool select mode, press start.
In color mode, the currently edited color button [A or B] has the color designated to it below
the A or B in the left there. You can switch the currently edited color by pressing the button
you want to edit. [only A an B for now] You can edit the Red, Green and Blue values of the color
by using the sliders on the right. The currently selected slider is white and the other two
are black. To switch between sliders, press up and down accordingly. To slide the currently
selected slider, press left to decrement and right to increment. The values for each color can be
from 0 - 31. that gives you 32*32*32, or 32768 different possible colors. The value [0-31]
is also displayed in a box to the right of each color. when you're done changing the colors,
press select to switch back to drawing mode and try out your colors. You can also (while in
drawing mode) press L to speed up the tool, and R to slow down.

What i want to include in later versions:

- A program that'll let you change the SRAM into a .tiff
- more tools such as paint bucket, spray painter, etc.
- zooming, scaling, rotation [all in mode 3]
- link support for 2 people editing the same image at the same time. That would rule.
- some other kinda peripheal support, such as GB printer or something
- sound of some kind. like a "thud" when you try to go past the edge of the screen, etc.
- perhaps multibooting for a 4 player drawfest type thingy

History:

- EBPP 0.2 released September 23, 2001. New Features:
	- Added:
		- the brush, eraser, line, rectangle and circle tools
		- SRAM saving
		- changed the color edit screen
	- Bug Fixes:
		- Only draw on half the screen, and the pen would go off the bottom of the screen
		  because i mixed up x and y [reported by CapNpants]
		- L and R keys were reversed
	- Removal:
		- L an R keys in drawing mode to change tool speed
- EBPP 0.1 released september 8, 2001. New features:
	Added:
	 	- a "pen" that can be speeded up/down
		- two colors simultaniously with A and B
		- edit color mode set the r,g and b for the A and B buttons [32768 possible colors]
- EBPP 0.05 released september 5, 2001. Features:
	Added:
		- draw red on a blue background using the d-pad
		- press select for title screen / color edit teaser

Open Source:

Everything i make is open source. that does not mean you can use my code as your own. it means
that you can use it if you give appropriate credit. thats all i ask. if you use some of my code
just put that some code was borrowed from The Black Frog [TheBlackFrog.8m.com] in the source if
its open source, and if its not, put it in the ROM itself. heck, put it in the ROM anyway :)
I commented the source as best as possible, but i never comment while coding, only when i release
so it might not make sense sometimes [to you or me], but it works.

Bugs:

If you find any bugs with this program, and it is the latest release [check my site], please
email me at ebpp@TheBlackFrog.8m.com with everything you know about the bug, and you'll get your
name in the source. 

Other:

All my demos and programs are availible with full source from www.TheBlackFrog.8m.com
That is also the home of Evil Bob's Pocket Painter and you can find out more about it [like
why i named it Evil Bob's Pocket Painter :P] by visiting my site.

If you want to contact me, i an usually in #gbadev on EFNET between 8 pm until 10 pm [GMT -6]
every weekday night. Otherwise email me: ebpp@TheBlackFrog.8m.com

Greetz:

Dovoto, Joat, JayC, Loco-san, SlasherX, 
Staring Monkey, TwinD, Fett, Larsby, Zer0,
jtafk, #gbadev on efnet, and #BlackFrog on EFNET