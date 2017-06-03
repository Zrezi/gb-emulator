¥CC = gcc
CXX = g++
ECHO = echo
RM = rm -f

CFLAGS = -Wall -ggdb
CXXFLAGS = -Wall -ggdb -std=c++11
LDFLAGS = 

BIN = main
OBJS = main.o cartridge.o cartridgesize.o cartridgetype.o memorymap.o input.o interrupt.o lcdinterrupt.o sprite.o z80.o

all: $(BIN)

$(BIN): $(OBJS)
	@$(ECHO) Linking $@
	@$(CXX) $^ -o $@ $(LDFLAGS)

-include $(OBJS:.o=.d)

%.o: %.c
	@$(ECHO) Compiling $<
	@$(CC) $(CFLAGS) -MMD -MF $*.d -c $<

%.o: %.cpp
	@$(ECHO) Compiling $<
	@$(CXX) $(CXXFLAGS) -MMD -MF $*.d -c $<

.PHONY: all clean clobber

clean:
	@$(ECHO) Removing all generated files
	@$(RM) *.o $(BIN) *.d core vgcore.* gmon.out
