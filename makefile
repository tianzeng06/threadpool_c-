CC=g++
CPPFLAGS=-Wall -pthread -std=c++11
EXEC=main
SOURCES=main.cpp threadpool.cpp condition.cpp
OBJS=$(SOURCES:%.cpp=%.o)
.PHONY:clean

$(EXEC):$(SOURCE)
	$(CC) -o $(EXEC) $(SOURCES) $(CPPFLAGS)

clean:
	rm -rf $(EXEC)
