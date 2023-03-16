CXX = g++
CC = gcc
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -lwiringPi -lm -O3
CFLAGS = -Wall -Wextra -pedantic -lwiringPi -lm -O3
LDFLAGS = -std=c++11 -Wall -Wextra -pedantic -lwiringPi -lm -O3

CPP_SRCS = main.cpp ../camera/camera.cpp ../gps/gps.cpp ../sensehat/sensehat.cpp
C_SRCS = ../sensehat/ICM20948.c
OBJS = $(CPP_SRCS:.cpp=.o) $(C_SRCS:.c=.o)
EXEC = main

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $(EXEC)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXEC) $(OBJS)