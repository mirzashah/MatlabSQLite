# Makefile for MatlabSQLite
# Mirza A. Shah

CC = /home/mas644/matlab/mex
CFLAGS =  COPTIMFLAGS='-O3 -DNDEBUG' -I. -I$(HELPERPROJECTS)/MaSUtilities -DLINUX
LFLAGS = -L../Debug -L$(HELPERPROJECTS)/Debug -lMaSUtilities -ldl
OUT = ../Debug/MatlabSQLite

OBJS =	MexEntryPoint.o \
			MatlabHelper.cpp \
			FunctionImplementations.cpp \
			sqlite3.c \
			SQLiteSimpleWrapper.cpp 
			

all:    buildInterface

buildInterface: $(OBJS)
	$(CC) $(CFLAGS) \
	-output $(OUT) \
	$(OBJS) \
	$(LFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) -c -o $*.o $*.cpp

clean:
	rm -f *.o $(OUT).* *~ 

