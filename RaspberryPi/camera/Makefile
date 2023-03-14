CXX = g++
CXXFLAGS = -Wall -O3 -std=c++17 
LDFLAGS = -std=c++17
SOURCES = test.cpp camera.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = camera

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
