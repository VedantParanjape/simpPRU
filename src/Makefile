CC := gcc
CSRCS := $(wildcard *.c) \
 	     $(wildcard ../third-party/cJSON/*.c) \
	     $(wildcard ../third-party/vec/*.c) \

ifeq ($(filter parser.tab.c, $(CSRCS)),)
	CSRCS := $(CSRCS) parser.tab.c
endif

ifeq ($(filter lex.yy.c, $(CSRCS)),)
	CSRCS := $(CSRCS) lex.yy.c
endif

OBJS := $(notdir $(CSRCS:.c=.o))	
TARGET := simppru
CFLAGS := -I./../include/ -I./../third-party/

all: bison flex $(TARGET)
	@echo All done

$(OBJS): $(CSRCS)
	$(CC) -c $^ $(CFLAGS)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@ $(CFLAGS)	
	rm $(OBJS)

bison: parser.y
	bison --defines=./../include/parser.tab.h parser.y

flex: lexer.l ./../include/parser.tab.h
	flex lexer.l

clean:
	rm $(TARGET) ./../include/parser.tab.h lex.yy.c parser.tab.c

.PHONY: all clean
