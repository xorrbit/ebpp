@cls
@zarmasm -Littleend -cpu ARM7TDMI boot.asm
@echo Step 1 of 3 complete
@zarmcc -c -Wall -Otime  -fpu none -Littleend -cpu ARM7TDMI -apcs /narrow/noswst ebpp.c  -o ebpp.o -errors errlog.txt
@echo Step 2 of 3 complete
@zarmlink -bin -first boot.o -map -ro-base 0x08000000 -rw-base 0x2000000 boot.o   ebpp.o   -o ebpp.gba
@echo Step 3 of 3 complete
@del *.o
@cls
@echo Step 1 of 3 complete
@echo Step 2 of 3 complete
@echo Step 3 of 3 complete
@echo -
@echo ERRORS:
@type errlog.txt|more
@echo END OF ERRORS