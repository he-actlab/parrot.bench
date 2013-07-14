COMPILE_MODE=NPU_VALIDATION

CC=g++
CFLAGS=-O3 -MD
C0FLAGS=-O1 -MD

INCLUDES=-I../tools -I../Npu 

LFLAGS=# -L/home/newhall/lib  -L../lib

LIBS=# -lmylib -lm

SRCS=$(wildcard *.c)

SRCS=dct.c      \
     encoder.c  \
     huffman.c  \
     jpeg.c     \
     quant.c    \
     rgbimage.c \

SRCS0=marker.c

# $(name:string1=string2)
# For each word in 'name' replace 'string1' with 'string2'
OBJS=$(SRCS:.c=.o)
OBJS0=$(SRCS0:.c=.o)

OUT_DIR=out
OUT=bench_$(NPU_MODE).out


.PHONY: depend clean

all: $(OUT)
	objdump --disassemble $(OUT_DIR)/$(OUT) > $(OUT_DIR)/$(OUT).asm
	readelf --relocs $(OUT_DIR)/$(OUT)

$(OUT): $(OBJS) $(OBJS0) 
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(OBJS0) $(LFLAGS) $(LIBS) -o $(OUT_DIR)/$(OUT)

# This is a suffix replacement rule for building .o's from .c's.
# It uses 'automatic variables' $<: the name of the prerequisite of
# the rule (a .c file) and $@: the name of the target of the rule
# (a .o file) 
%.o: %.c
	$(CC) -D$(NPU_MODE) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJS0): $(SRCS0)
	$(CC) -D$(NPU_MODE) $(C0FLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) *.o *.d

dist:
	$(RM) *.o *.d $(OUT_DIR)/*

test:
	$(OUT_DIR)/$(OUT)

