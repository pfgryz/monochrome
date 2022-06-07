CC=gcc
ASMBIN=nasm
CC_FILENAME=main
ASM_FILENAME=monochrome
OUTPUT_FILENAME=main
all: asm cc cc_image link
asm:
	$(ASMBIN) -o $(ASM_FILENAME).o -f elf -g -l $(ASM_FILENAME).lst $(ASM_FILENAME).asm
cc:
	$(CC) -m32 -c -g -O0 -std=c99 $(CC_FILENAME).c &> errors.txt
cc_image:
	$(CC) -m32 -c -g -O0 -std=c99 image.c &> errors.txt
link:
	$(CC) -m32 -g -std=c99 -o $(OUTPUT_FILENAME) $(CC_FILENAME).o $(ASM_FILENAME).o image.o
clean:
	rm *.o
	rm $(OUTPUT_FILENAME)
	rm errors.txt
	rm $(ASM_FILENAME).lst
