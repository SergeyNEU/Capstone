CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -lwiringPi -lm -O3
LDFLAGS = -std=c++11 -Wall -Wextra -pedantic -lwiringPi -lm -O3

SRCS = main.cpp ../camera/camera.cpp ../gps/gps.cpp ../sensehat/sensehat.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = main

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $(EXEC)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(EXEC) $(OBJS)