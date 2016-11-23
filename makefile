.SUFFIXES: .cpp .o

CC=g++

SRCS=Thread.cpp\
			main.cpp

OBJS=$(SRCS:.cpp=.o)
EXEC=lockfree

all: $(OBJS)
	$(CC) -o $(EXEC) $(OBJS) -lpthread
	@echo '----------------ok----------------'
	
.cpp.o:
	$(CC) -std=c++0x -Wall -g -o $@ -c $< -I./
	
clean:
	rm -f $(OBJS)
	rm -f core*