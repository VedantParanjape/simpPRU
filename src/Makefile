# simpru compilation stuff
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

# PRU static library generation stuff for rpmsg
PRUCC := pru-gcc
PRUCSRCS  := $(filter-out ../generated_code/temp.c, $(wildcard ../generated_code/*.c))
PRUOBJS := $(notdir $(PRUCSRCS:.c=.o))
PRUTARGET0 := libprurpmsg0.a
PRUTARGET1 := libprurpmsg1.a
PRUCFLAGS := -I./../generated_code/include/ -I./../generated_code/include/am335x

# Default rule
all: bison flex $(TARGET) $(PRUTARGET0) $(PRUTARGET1)
	@echo All done

# simpru rules
$(OBJS): $(CSRCS)
	$(CC) -c $^ $(CFLAGS)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@ $(CFLAGS)
	rm $(OBJS)

bison: parser.y
	bison --defines=./../include/parser.tab.h parser.y

flex: lexer.l ./../include/parser.tab.h
	flex lexer.l

# prugcc rpmsg generation stuff
define generate_object = 
	$(PRUCC) -c $(PRUCSRCS) $(PRUCFLAGS) -mmcu=am335x.pru$(1)
endef

$(PRUTARGET0): 
	$(call generate_object,0)
	$(PRUCC)-ar -rc $@ $(PRUOBJS)
	rm $(PRUOBJS)
	mv $(PRUTARGET0) ../lib/pru/.

$(PRUTARGET1): 
	$(call generate_object,1) 
	$(PRUCC)-ar -rc $@ $(PRUOBJS)
	rm $(PRUOBJS)
	mv $(PRUTARGET1) ../lib/pru/.

clean:
	rm $(TARGET) ./../include/parser.tab.h lex.yy.c parser.tab.c

.PHONY: all clean
