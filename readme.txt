Evil Bob's Pocket Painter 0.1

by The Black Frog
http://www.TheBlackFrog.8m.com

from the source:

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

READ THE SOURCE, it has all the info on EBPP, including hostory, greetz, open source info, etc.
READ THE SOURCE
READ THE SOURCE

ebpp.c is the SOURCE

READ IT


files:

ebpp.c          - Main C files with all the code, and lotsa info in the beginning about ebpp
ebpp.gba        - Evil Bob's Pocket Painet ROM
objects.h       - Sprite data made with p.bat
objects.pcx     - PCX graphic file with all the sprites
coloredit.h     - Color Editor screen data made with bimbo
coloredit.bmp   - bitmap of the color editor screen
fixh.exe        - my own fixh program that fixes up the .h files created with pcx2gba/pcx2sprite by dovoto
pcx2sprite.exe  - dovoto's program to convert a pcx to a .h file with the sprite data and palette
gba.h		- Eloist's GBA header file with defines and such
boot.asm	- dovoto's boot.asm [?]
sprite.h	- dovoto's sprite defines header file
screenmode.h	- dovoto's screenmode defines header file
make.bat	- a batch file to compile ebpp using arm sdt
keypad.h	- dovoto's keypad defines header file
p.bat		- a batch file that runs pcx2sprite and fixh on a file
errlog.txt	- contains the errors report from the compiler
readme.txt 	- this file


If there are more files that that, you got it from a site other than TheBlackFrog.8m.com
That is okay, as i like to distribute as much as possible, but TheBlackFrog.8m.com will
always have the latest version first.