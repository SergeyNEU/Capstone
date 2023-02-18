CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic `pkg-config --cflags opencv4`
LDFLAGS = -lwiringPi -lm -O3 `pkg-config --libs opencv4`

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